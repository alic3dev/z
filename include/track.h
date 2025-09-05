#ifndef __track_h
#define __track_h

#include <cer0_phase.h>

struct track;

extern struct track* track_current;
extern struct track* track_upcoming;

extern float sample_rate;

struct track_lane;

struct track {
  char* name;

  float speed;
  float length;

  unsigned char length_lanes;
  struct track_lane* lanes;

  float* note_table;
  unsigned int length_note_table;

  unsigned char* scale;
  unsigned char length_scale;

  unsigned char key;

  unsigned char complete;
};

struct track_note {
  float time;
  float value;
};

struct track_lane {
  unsigned long int index_note;

  unsigned long int length_notes;
  struct track_note* notes;

  struct cer0_phase phase;
};

void tracks_initialize();

void track_generate(
  struct track*
);

void track_destroy(
  struct track*
);

#endif
