#include "input.h"

int key_to_note(Key key) {
    switch(key) {
        case KEY_Q: return 60; // C4
        case KEY_2: return 61; // C#4
        case KEY_W: return 62; // D4
        case KEY_3: return 63; // D#4
        case KEY_E: return 64; // E4
        case KEY_R: return 65; // F4
        case KEY_5: return 66; // F#4
        case KEY_T: return 67; // G4
        case KEY_6: return 68; // G#4
        case KEY_Y: return 69; // A4
        case KEY_U: return 71; // B4

        case KEY_I: return 72; // C5
        case KEY_9: return 73; // C#5
        case KEY_O: return 74; // D5
        case KEY_0: return 75; // D#5
        case KEY_L_BRACKET: return 76; // E5
        case KEY_R_BRACKET: return 77; // F5
    }

    return 0;
}
