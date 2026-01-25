#include "main.h"

void main(int argc, char** argv) {
    printf("Morgan v%s by Wolfyxon\n", VERSION);
    printf("https://github.com/Wolfyxon/morgan\n\n");
    printf("Starting...\n");
    
    input_init();    
    midi_init();

    printf("Ready!\n");
    printf("Select the \"Morgan MIDI\" input port in your DAW.\n");
    
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
