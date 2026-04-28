#ifndef __z_track_parameters_h
#define __z_track_parameters_h

#include <cer0_attack_sustain_decay_release.h>
#include <cer0_signal.h>

#include <rand_source_type.h>

#define z_track_parameters_length_scales_default 0x05
#define z_track_parameters_length_signals_default cer0_length_signals

struct z_track_parameters {
  const unsigned char** scales;
  unsigned char* scales_length;
  unsigned char length_scales;

  unsigned int track_length_lanes_minimum;
  unsigned int track_length_lanes_maximum;

  float frequency_root;

  char octave_minimum;
  char octave_maximum;

  enum cer0_signal signals[
    z_track_parameters_length_signals_default
  ];

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
