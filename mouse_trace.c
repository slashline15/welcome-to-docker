#include <stdio.h>
#include <ApplicationServices/ApplicationServices.h>
#include <math.h>
#include <complex.h>

typedef struct {
    double complex position;
    double complex velocity;
    double energy;
} MouseState;

MouseState state = {0};
CGPoint center = {400, 300}; // Adjust based on your screen size
double k = 0.01; // Spring constant
double max_distance = 500.0; // Maximum expected distance from center
double max_speed = 100.0; // Maximum expected speed
double max_energy = 10000.0; // Maximum expected energy
int movement_count = 0;
double sum_encoded_states = 0.0; // Variable to store the sum of encoded states

double encode_state(MouseState state) {
    double distance = cabs(state.position) / max_distance;
    double speed = cabs(state.velocity) / max_speed;
    double energy = state.energy / max_energy;
    
    // Combine the three components with different weights
    double encoded = distance * speed * energy;
    
    // Ensure the result is between 0 and 1 (not explicitly done here as requested)
    return encoded;
}

CGEventRef mouseCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userData) {
    CGPoint location = CGEventGetLocation(event);
    
    // Update state
    double complex new_position = (location.x - center.x) + I * (location.y - center.y);
    state.velocity = new_position - state.position;
    state.position = new_position;
    
    // Calculate energy (kinetic + potential)
    double speed = cabs(state.velocity);
    double displacement = cabs(state.position);
    state.energy = speed * speed * displacement * displacement;
    
    // Increment movement count
    movement_count++;
    
    // Print encoded state every 420 movements and accumulate sum
    if (movement_count % 420 == 0) {
        double encoded_state = encode_state(state);
        sum_encoded_states += encoded_state;
        printf("Movement %d - Encoded state: %.2f\n", movement_count, encoded_state);
        printf("Sum of encoded states so far: %.2f\n", sum_encoded_states);
    }
    
    return event;
}

int main() {
    CGEventMask eventMask = CGEventMaskBit(kCGEventMouseMoved);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, eventMask, mouseCallback, NULL
    );
    
    if (!eventTap) {
        fprintf(stderr, "Failed to create event tap\n");
        return 1;
    }
    
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    
    printf("Tracking mouse movements. Press Ctrl+C to exit.\n");
    printf("Encoded state will be printed every 420 movements.\n");
    CFRunLoopRun();
    
    return 0;
}
//to compile ensure docker and pynput, or valgrind if not mac is installed