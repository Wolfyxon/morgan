#include <alsa/asoundlib.h>

void alsa_init();
void alsa_note_on(int key, int8_t velocity);
void alsa_note_off(int key, int8_t velocity);
void alsa_flush();
