#include "linux.h"

SessionType linux_get_session_type() {
    char* session = getenv("XDG_SESSION_TYPE");

    if(strcmp(session, "x11") == 0) {
        return SESSION_X11;
    }

    if(strcmp(session, "wayland") == 0) {
        return SESSION_WAYLAND;
    }

    fprintf(stderr, "warning: Unknown XDG_SESSION_TYPE: %s", session);
    return SESSION_UNKNOWN;
}
