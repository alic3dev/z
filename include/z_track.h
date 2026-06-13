#ifndef __z_track_h
#define __z_track_h

#include <z_track_lane.h>
#include <z_track_parameters.h>

#include <cer0_effect.h>

#include <rand_result.h>
#include <rand_source.h>
#include <rand_source_divisive_data.h>

struct z_track {
  char* name;

  float bpm;
  float length;

  float progress;

  unsigned int length_lanes;
  struct z_track_lane* lanes;

  float* note_table;
  unsigned int length_note_table;

  unsigned char range_octave;

  const unsigned char* scale;
  unsigned char length_scale;

  unsigned char key;

  char* char_array_seed;
  unsigned int length_char_array_seed;

  struct rand_parameters rand_parameters;
  struct rand_result rand_result;
  struct rand_source rand_source;

  struct cer0_effect* effects;
  unsigned char length_effects;
};

void z_track_generate(
  struct z_track*,
  struct z_track_parameters*,
  float
);

void z_track_destroy(
  struct z_track*
);

#endif
