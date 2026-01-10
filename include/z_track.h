#ifndef __z_track_h
#define __z_track_h

#include <z_track_lane.h>
#include <z_track_parameters.h>

#include <rand_result.h>
#include <rand_source.h>
#include <rand_source_divisive_data.h>

struct z_track {
  char* name;

  float speed;
  float length;

  float progress;

  unsigned char length_lanes;
  struct z_track_lane* lanes;

  float* note_table;
  unsigned int length_note_table;

  unsigned char range_octave;

  const unsigned char* scale;
  unsigned char length_scale;

  unsigned char key;

  struct rand_parameters rand_parameters;
  struct rand_result rand_result;
  struct rand_source rand_source;
  struct rand_source_parameters rand_source_parameters;
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
