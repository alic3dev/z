#ifndef __z_track_parameters_h
#define __z_track_parameters_h

#include <z_track_lane_type.h>

#include <cer0_attack_sustain_decay_release.h>
#include <cer0_signal.h>

#include <rand_source_type.h>

#define z_track_parameters_length_scales_default 0x05
#define z_track_parameters_length_signals_default cer0_length_signals
#define z_track_parameters_length_types_default z_track_lane_types_length

struct z_track_parameters {
  const unsigned char** scales;
  unsigned char* scales_length;
  unsigned char length_scales;

  unsigned int track_length_lanes_minimum;
  unsigned int track_length_lanes_maximum;

  float frequency_root;

  char octave_minimum;
  char octave_maximum;

  int* signals;
  unsigned char length_signals;
  
  unsigned char* types;
  unsigned char length_types;
  
  float track_bpm_minimum;
  float track_bpm_maximum;

  float track_length_multiplier;

  float oscillator_amplitude_minimum;
  float oscillator_amplitude_maximum;

  float note_amplitude_minimum;
  float note_amplitude_maximum;

  struct cer0_attack_sustain_decay_release_parameters attack_sustain_decay_release_parameters_minimum;
  struct cer0_attack_sustain_decay_release_parameters attack_sustain_decay_release_parameters_maximum;
};

extern const unsigned char* z_track_parameters_scales_defaults[
  z_track_parameters_length_scales_default
];

extern unsigned char z_track_parameters_lengths_scales_defaults[
  z_track_parameters_length_scales_default
];

extern const unsigned char z_track_parameters_signals_defaults[
  z_track_parameters_length_signals_default
];

extern const unsigned char z_track_parameters_types_defaults[
  z_track_parameters_length_types_default
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
