#include "main.h"

int main(int argc, char** argv) {
    printf("Morgan v%s by Wolfyxon\n", VERSION);
    printf("https://github.com/Wolfyxon/morgan\n\n");
    printf("Starting...\n");
    
    input_init();    
    midi_init();
    
    printf("Ready!\n");
    printf("Select the \"Morgan MIDI\" input port in your DAW.\n");
    
    list_keyboards();
    main_loop();
    
    return 0;
}

void main_loop() {
     while(1) {
        KeyEvent key_ev = input_poll_event();

        if(key_ev.key == KEY_UNKNOWN) {
            continue;
        }

        printf("Key %d %d from %d \n", key_ev.key, key_ev.pressed, key_ev.device);

        int note = key_to_note(key_ev.key);

        if(note == 0) {
            continue;
        }

        if(key_ev.pressed) {
            midi_note_on(note, 50);
        } else {
            midi_note_off(note, 50);
        }
    }
}

void list_keyboards() {
    int length = 0;
    Keyboard* keyboards = input_get_keyboards(&length);

    printf("Detected keyboards: \n");

    for(int i = 0; i < length; i++) {
        Keyboard kb = keyboards[i];

        printf("  %d: %s \n", kb.id, kb.name);
    }
}