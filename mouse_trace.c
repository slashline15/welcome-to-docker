#include <stdio.h>
#include <ApplicationServices/ApplicationServices.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    double complex position;
    double complex velocity;
    double energy;
} MouseState;

MouseState state = {0};
CGPoint center = {300, 300}; // Adjust based on your screen size
double max_distance = 10.0;
double max_speed = 100.0;
double max_energy = 100.0;
int movement_count = 0;
double sum_encoded_states = 0.0;
bool signature_complete = false;
char password[100] = "";

#define NUM_ATTEMPTS 5
double signature_sums[NUM_ATTEMPTS];
int current_attempt = 0;

double encode_state(MouseState state) {
    double distance = cabs(state.position) / max_distance;
    double speed = cabs(state.velocity) / max_speed;
    double energy = state.energy / max_energy;
    return distance + speed + energy;
}

void reset_signature_capture() {
    state = (MouseState){0};
    movement_count = 0;
    sum_encoded_states = 0.0;
    signature_complete = false;
}

CGEventRef eventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userData) {
    if (signature_complete) {
        return event;
    }

    if (type == kCGEventKeyDown) {
        CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        if (keyCode == 49) { // Spacebar
            signature_complete = true;
            signature_sums[current_attempt] = sum_encoded_states;
            printf("\nSignature capture complete for attempt %d.\n", current_attempt + 1);
        }
    } else if (type == kCGEventMouseMoved) {
        CGPoint location = CGEventGetLocation(event);
        
        double complex new_position = (location.x - center.x) + I * (location.y - center.y);
        state.velocity = new_position - state.position;
        state.position = new_position;
        double speed = cabs(state.velocity);
        double displacement = cabs(state.position);
        state.energy = speed*speed + displacement;
        
        movement_count++;
        
        if (movement_count % 10 == 0) {
            double encoded_state = encode_state(state);
            sum_encoded_states += encoded_state;
            printf("Movement %d - Encoded state: %.2f\n", movement_count, encoded_state);
            printf("Sum of encoded states: %.2f\n", sum_encoded_states);
            
            FILE *f = fopen("signature_profile.txt", "a");
            if (f != NULL) {
                fprintf(f, "Attempt %d - Encoded state: %.2f\n", current_attempt + 1, encoded_state);
                fclose(f);
            }
        }
    }
    
    return event;
}

int main() {
    CGEventMask eventMask = CGEventMaskBit(kCGEventMouseMoved) | CGEventMaskBit(kCGEventKeyDown);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, eventMask, eventCallback, NULL
    );
    
    if (!eventTap) {
        fprintf(stderr, "Failed to create event tap\n");
        return 1;
    }
    
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    
    for (current_attempt = 0; current_attempt < NUM_ATTEMPTS; current_attempt++) {
        printf("\nStart moving the mouse to create your signature (Attempt %d/%d). Press spacebar when done.\n", current_attempt + 1, NUM_ATTEMPTS);
        reset_signature_capture();

        while (!signature_complete) {
            CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.1, false);
        }
    }

    CGEventTapEnable(eventTap, false);
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CFRelease(runLoopSource);
    CFRelease(eventTap);

    // Calculate range
    double min_sum = signature_sums[0];
    double max_sum = signature_sums[0];
    for (int i = 1; i < NUM_ATTEMPTS; i++) {
        if (signature_sums[i] < min_sum) min_sum = signature_sums[i];
        if (signature_sums[i] > max_sum) max_sum = signature_sums[i];
    }

    printf("\nSignature range: %.2f to %.2f\n", min_sum, max_sum);
    printf("Enter your password and press Enter when done: ");
    fflush(stdout);
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0; // Remove trailing newline

    // Here you would typically hash the password and store it securely
    // For demonstration, we'll just print the length
    //printf("Password length: %lu\n", strlen(password));

    printf("\nSignature profile and password captured.\n");
    printf("In future logins, if the signature encoding falls outside %.2f to %.2f, additional ID checks will be required.\n", min_sum, max_sum);

    return 0;
}

//compile: gcc -framework ApplicationServices mouse_trace.c -o mouse_trace #make sure your docker and stuff is configured 
