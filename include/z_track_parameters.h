#ifndef __z_track_parameters_h
#define __z_track_parameters_h

#include <cer0_signal.h>

#include <rand_source_type.h>

#define z_track_parameters_length_signals_default cer0_length_signals

struct z_track_parameters {
  const unsigned char** scales;
  unsigned char* scales_length;
  unsigned char length_scales;

  unsigned char track_length_lanes_minimum;
  unsigned char track_length_lanes_maximum;

  float frequency_root;

  char octave_minimum;
  char octave_maximum;

  enum cer0_signal signals[
    z_track_parameters_length_signals_default
  ];

  float track_length_multiplier;
  float track_speed_multiplier;

  float note_amplitude_minimum;
  float note_amplitude_maximum;

  float note_attack_minimum;
  float note_attack_maximum;

  float note_release_minimum;
  float note_release_maximum;

  enum rand_source_type rand_source_type;

  unsigned char allocated_scales;
};

#define z_track_parameters_length_scales_default 5

extern const unsigned char* z_track_scales_defaults[
  z_track_parameters_length_scales_default
];

extern unsigned char z_track_lengths_scales_defaults[
  z_track_parameters_length_scales_default
];

extern const struct z_track_parameters z_track_parameters_defaults;

void z_track_parameters_initialize(
  struct z_track_parameters*
);

void z_track_parameters_initialize_defaults(
  struct z_track_parameters*
);

void z_track_parameters_destroy(
  struct z_track_parameters*
);

#endif
