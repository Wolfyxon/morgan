#include "../input.h"
#include "wininput.h"

void input_init() {
    win_init();
    win_loop();
}

Keyboard* input_get_keyboards(int* length) {
    return win_get_keyboards(length);
}
