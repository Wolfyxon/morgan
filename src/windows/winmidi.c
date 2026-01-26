#include "winmidi.h"

HMIDIOUT win_midi_device;

void win_midi_init() {
    MMRESULT open_error = midiOutOpen(&win_midi_device, 0, 0, 0, CALLBACK_NULL);
    
    if(open_error != MMSYSERR_NOERROR) {
        fprintf(stderr, "error: Failed to open MIDI device. Error: %d \n", open_error);
        exit(1);
    }
}

void win_midi_note_on(int key, int8_t velocity) {
    WinMidiMessage msg;

    msg.data[0] = WIN_MIDI_NOTE_ON;
    msg.data[1] = key;
    msg.data[2] = velocity;
    msg.data[3] = 0;

    win_midi_send(msg);
}

void win_midi_note_off(int key) {
    WinMidiMessage msg;

    msg.data[0] = WIN_MIDI_NOTE_ON;
    msg.data[1] = key;
    msg.data[2] = 0;
    msg.data[3] = 0;

    win_midi_send(msg);
}

void win_midi_send(WinMidiMessage message) {
    MMRESULT send_err = midiOutShortMsg(win_midi_device, message.word);

    if(send_err != MMSYSERR_NOERROR) {
        fprintf(stderr, "error: Failed to send MIDI message. Error: %d \n", send_err);
    }
}
