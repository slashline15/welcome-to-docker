#include <stdio.h>
#include <ApplicationServices/ApplicationServices.h>

void mouseCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userData) {
    CGPoint location = CGEventGetLocation(event);
    printf("Mouse position: (%f, %f)\n", location.x, location.y);
}

int main() {
    CGEventMask eventMask = CGEventMaskBit(kCGEventMouseMoved);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, mouseCallback, NULL
    );
    
    if (!eventTap) {
        fprintf(stderr, "Failed to create event tap\n");
        return 1;
    }
    
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    
    printf("Tracking mouse movements. Press Ctrl+C to exit.\n");
    CFRunLoopRun();
    
    return 0;
}