#include "../input.h"
#include "../defines.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>

typedef struct {
    Display* display;
    int opcode;
} X11Context;

void x11_init();
void x11_assert_ready();
void x11_flush();
KeyEvent x11_poll_event();
Keyboard* x11_get_keyboards(int* length);
Key x11_input_to_key(int code);
