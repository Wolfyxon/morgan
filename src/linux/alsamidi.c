#include "alsamidi.h"

snd_seq_event_t alsa_event;
snd_seq_t* alsa_seq = NULL;

void alsa_init() {
    snd_seq_open(&alsa_seq, "default", SND_SEQ_OPEN_OUTPUT, 0);

    if(!alsa_seq) {
        fprintf(stderr, "error: Failed to open Alsa MIDI sequencer");
        exit(1);
    }

	snd_seq_set_client_name(alsa_seq, "Morgan MIDI port");
	snd_seq_create_simple_port(alsa_seq, "out", SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ | SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_APPLICATION);
	
    snd_seq_ev_clear(&alsa_event);
    snd_seq_ev_set_direct(&alsa_event);
    snd_seq_ev_set_subs(&alsa_event);
}

void alsa_note_on(int key, int8_t velocity) {
    snd_seq_ev_set_noteon(&alsa_event, 0, key, velocity);
    alsa_flush();
}

void alsa_note_off(int key, int8_t velocity) {
    snd_seq_ev_set_noteoff(&alsa_event, 0, key, velocity);
    alsa_flush();
}

void alsa_flush() {
    snd_seq_event_output(alsa_seq, &alsa_event);
    snd_seq_drain_output(alsa_seq);
}
