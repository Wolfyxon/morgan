#pragma once

#include <stdbool.h>

// what am I doing
typedef enum {
    KEY_UNKNOWN,

    // letters
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    // numbers
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    // other
    KEY_COLON,
    KEY_QUOTE,
    KEY_LT, // <
    KEY_GT, // >
    KEY_SLASH,
    KEY_L_BRACKET,
    KEY_R_BRACKET,
    KEY_MINUS,
    KEY_EQUALS
} Key;

typedef struct {
    Key key;
    int device;
    bool pressed;
} KeyEvent;

typedef struct {
    int id;
    char* name;
} Keyboard;

void input_init();
KeyEvent input_poll_event();
Keyboard* input_get_keyboards(int* length);
