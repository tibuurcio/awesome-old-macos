// fkey.c
// http://osxbook.com
//
// Complile using the following command line:
//     gcc -Wall -o fkey fkey.c -framework ApplicationServices
//
// You need superuser privileges to create the event tap, unless accessibility
// is enabled. To do so, select the "Enable access for assistive devices"
// checkbox in the Universal Access system preference pane.

#include <sys/time.h>
#include <ApplicationServices/ApplicationServices.h>


CGKeyCode prevKeycode;
struct timespec start, end;
uint64_t delta_us;

// This callback will be invoked every time there is a keystroke.
//
CGEventRef myCGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    // Paranoid sanity check.
    if ((type != kCGEventKeyDown))// && (type != kCGEventKeyUp))
        return event;
    
    // The incoming keycode.
    CGKeyCode keycode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    fprintf(stdout, "The keycode is: %d\n", (int)keycode);

    UniCharCount actualStringLength;
    UniChar unicodeString[4]; // Unicode string can contain up to 4 characters

    CGEventKeyboardGetUnicodeString(event, 4, &actualStringLength, unicodeString);

    // Ensure that the actualStringLength is not 0 and the unicodeString is printable
    if (actualStringLength > 0 && isprint(unicodeString[0])) {
        // Print the first character of the Unicode string
        fprintf(stdout, "The key pressed is: %c\n", unicodeString[0]);
    } else {
        // Print the keycode if Unicode string is not available
        fprintf(stdout, "The key pressed has no printable character. KeyCode: %d\n", (int)keycode);
    }
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    fprintf(stdout, "Delta: %d\n", delta_us);

    if (keycode == prevKeycode && delta_us < 200 * 1000) {
        fprintf(stdout, "Duplicated detected with delta %d, skipping...\n", delta_us);
        CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, (int64_t)-1);
        return event;
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    prevKeycode = keycode;
    return event;
}

int main(void) {
    CFMachPortRef      eventTap;
    CGEventMask        eventMask;
    CFRunLoopSourceRef runLoopSource;
    
    // Create an event tap. We are interested in key presses.
    eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventKeyUp));
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0,
                                eventMask, myCGEventCallback, NULL);
    if (!eventTap) {
        fprintf(stderr, "failed to create event tap\n");
        exit(1);
    }
    
    gettimeofday(&start, NULL);
    
    // Create a run loop source.
    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    
    // Add to the current run loop.
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    
    // Enable the event tap.
    CGEventTapEnable(eventTap, true);
    
    // Set it all running.
    CFRunLoopRun();
    
    // In a real program, one would have arranged for cleaning up.
    exit(0);
}
