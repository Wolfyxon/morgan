#include "../midi.h"
#include "alsamidi.h"

void midi_init() {
    alsa_init();
}

void midi_note_on(int key, int8_t velocity) {
    alsa_note_on(key, velocity);
}

void midi_note_off(int key, int8_t velocity) {
    alsa_note_off(key, velocity);
}
