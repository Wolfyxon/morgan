#include "../input.h"
#include "linux.h"

void input_init() {
    if(linux_get_session_type() != SESSION_X11) {
        fprintf(stderr, "warning: Only Xorg is officially supported. You may run into issues.");
    }

    X11Context ctx = x11_init();

    while(1) {
        x11_poll_events(&ctx);
    }
}
