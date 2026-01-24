#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "x11input.h"

typedef enum {
    SESSION_X11,
    SESSION_WAYLAND,
    SESSION_UNKNOWN
} SessionType;

SessionType linux_get_session_type();
