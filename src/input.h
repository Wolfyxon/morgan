#pragma once

#include <stdbool.h>

// what am I doing
typedef enum {
    // letters
    KEY_A = (1UL << 0),
    KEY_B = (1UL << 1),
    KEY_C = (1UL << 2),
    KEY_D = (1UL << 3),
    KEY_E = (1UL << 4),
    KEY_F = (1UL << 5),
    KEY_G = (1UL << 6),
    KEY_H = (1UL << 7),
    KEY_I = (1UL << 8),
    KEY_J = (1UL << 9),
    KEY_K = (1UL << 10),
    KEY_L = (1UL << 11),
    KEY_M = (1UL << 12),
    KEY_N = (1UL << 13),
    KEY_O = (1UL << 14),
    KEY_P = (1UL << 15),
    KEY_Q = (1UL << 16),
    KEY_R = (1UL << 17),
    KEY_S = (1UL << 18),
    KEY_T = (1UL << 19),
    KEY_U = (1UL << 20),
    KEY_V = (1UL << 21),
    KEY_W = (1UL << 22),
    KEY_X = (1UL << 23),
    KEY_Y = (1UL << 24),
    KEY_Z = (1UL << 25),
    // numbers
    KEY_0 = (1UL << 26),
    KEY_1 = (1UL << 27),
    KEY_2 = (1UL << 28),
    KEY_3 = (1UL << 29),
    KEY_4 = (1UL << 30),
    KEY_5 = (1UL << 31),
    KEY_6 = (1UL << 32),
    KEY_7 = (1UL << 33),
    KEY_8 = (1UL << 34),
    KEY_9 = (1UL << 35),
    // other
    KEY_COLON = (1UL << 36),
    KEY_QUOTE = (1UL << 37),
    KEY_LT = (1UL << 38), // <
    KEY_GT = (1UL << 39), // >
    KEY_SLASH = (1UL << 40),
    KEY_L_BRACKET = (1UL << 41),
    KEY_R_BRACKET = (1UL << 42),
    KEY_MINUS = (1UL << 43),
    KEY_EQUALS = (1UL << 44)
} Key;

extern unsigned long global_key_mask;

void input_init();
