#include "main.h"

void main(int argc, char** argv) {
    printf("Running\n");
    
    input_init();
    midi_init();

    while(1) {
        KeyEvent key_ev = input_poll_event();

        if(key_ev.key == KEY_UNKNOWN) {
            continue;
        }

        printf("Key %d %d from %d \n", key_ev.key, key_ev.pressed, key_ev.device);

        if(key_ev.pressed) {
            midi_note_on(60, 50);
        } else {
            midi_note_off(60, 50);
        }
    }
}
