#include "../input.h"
#include "linux.h"

void input_init() {
    if(linux_get_session_type() != SESSION_X11) {
        fprintf(stderr, "warning: Only Xorg is officially supported. You may run into issues.\n");
    }

    x11_init();
}

void input_flush() {
    x11_flush();
}

KeyEvent input_poll_event() {
    return x11_poll_event();
}

Keyboard* input_get_keyboards(int* length) {
    return x11_get_keyboards(length);
}
