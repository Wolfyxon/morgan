#include "../input.h"
#include "wininput.h"

void input_init() {
    win_init();
    win_loop();
}
