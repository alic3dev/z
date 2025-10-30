#ifndef __track_h
#define __track_h

#include <cer0_synthesizer.h>

#include <rand_result.h>
#include <rand_source.h>

#define cer0_length_track_scales 4

extern float sample_rate;

struct track_lane;

struct track {
  char* name;

  float seed;
  unsigned char buffer_seed[
    sizeof(float)
  ];

  float speed;
  float length;

  float progress;

  unsigned char length_lanes;
  struct track_lane* lanes;

  float* note_table;
  unsigned int length_note_table;

  unsigned char range_octave;

  unsigned char* scale;
  unsigned char length_scale;

  unsigned char key;

  unsigned char complete;

  struct rand_parameters rand_parameters;
  struct rand_result rand_result;
  struct rand_source rand_source;
  struct rand_source_parameters rand_source_parameters;
};

struct track_note {
  float time;
  float value;
};

struct track_lane {
  unsigned long int index_note;

  unsigned long int length_notes;
  struct track_note* notes;

  struct cer0_synthesizer synthesizer;
};

void track_generate(
  struct track*
);

void track_destroy(
  struct track*
);

#endif
