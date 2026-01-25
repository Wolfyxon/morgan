#include "../input.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>

typedef struct {
    Display* display;
    int opcode;
} X11Context;

void x11_init();
KeyEvent x11_poll_event();
Key x11_input_to_key(int code);
