#include <stdlib.h>
#include <stdint.h>

#define DEFAULT_OCTAVE 3
#define INVALID_NOTE -1024

void midi_init();
void midi_note_on(int key, int8_t velocity);
void midi_note_off(int key, int8_t velocity);
