#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>

typedef struct {
    Display* display;
    int opcode;
} X11Context;

X11Context x11_init();
void x11_poll_events(X11Context* ctx);
