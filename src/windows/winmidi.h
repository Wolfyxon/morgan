#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <mmsystem.h>

#define WIN_MIDI_NOTE_ON 0x90

typedef union {
    unsigned long word;
    unsigned char data[4];
} WinMidiMessage;

void win_midi_init();
void win_midi_note_on(int key, int8_t velocity);
void win_midi_note_off(int key);
void win_midi_send(WinMidiMessage message);
