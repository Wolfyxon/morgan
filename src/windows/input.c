#include "../input.h"
#include "wininput.h"

void input_init() {
    win_init();
}

void input_flush() {
    
}

Keyboard* input_get_keyboards(int* length) {
    return win_get_keyboards(length);
}

KeyEvent input_poll_event() {
    return win_poll_key_event();
}
