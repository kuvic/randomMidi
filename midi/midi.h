#ifndef MIDI_H
#define MIDI_H

struct midi_header {
	char mthd[4];
	uint32_t header_length;
	uint16_t format;
	uint16_t n_tracks;
	int16_t division;
};

struct midi_track {
	char mtrk[4];
	uint32_t track_length;
};

struct midi_event {
	char status;
	char note_n;
	char note_v;
};

#endif
