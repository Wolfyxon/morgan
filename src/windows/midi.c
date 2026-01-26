#include "../midi.h"
#include "winmidi.h"

void midi_init() {
    win_midi_init();
}

void midi_note_on(int key, int8_t velocity) {
    win_midi_note_on(key, velocity);
}

void midi_note_off(int key, int8_t velocity) {
    win_midi_note_off(key);
}
