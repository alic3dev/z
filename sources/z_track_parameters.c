#include <z_track_parameters.h>

#include <cer0_frequency_root.h>
#include <cer0_scale.h>
#include <cer0_signal.h>

#include <rand_source_type.h>

#include <stdlib.h>

const unsigned char* z_track_scales_defaults[
  z_track_parameters_length_scales_default
] = {
  cer0_scale_notes_major_pentatonic,
  cer0_scale_notes_harmonic_major,
  cer0_scale_notes_major_pentatonic,
  cer0_scale_notes_harmonic_minor,
  cer0_scale_notes_neapolitan_major
};

unsigned char z_track_scales_lengths_defaults[
  z_track_parameters_length_scales_default
] = {
  cer0_scale_length_major_pentatonic,
  cer0_scale_length_harmonic_major,
  cer0_scale_length_major_pentatonic,
  cer0_scale_length_harmonic_major,
  cer0_scale_length_neapolitan_major
};

const struct z_track_parameters z_track_parameters_defaults = {
  .scales = z_track_scales_defaults,
  .scales_length = z_track_scales_lengths_defaults,
  .length_scales = z_track_parameters_length_scales_default,
  .track_length_lanes_minimum = 2,
  .track_length_lanes_maximum = 8,
  .frequency_root = cer0_frequency_root_scientific,
  .octave_minimum = 1,
  .octave_maximum = 4,
  .signals = {
    sine,
    sine_alice,
    sine,
    sine_alice,
    sine,
    sine_alice
  },
  .track_length_multiplier = 4.19f,
  .track_speed_multiplier = 0.114f,
  .oscillator_amplitude_minimum = 0.4f,
  .oscillator_amplitude_maximum = 1.0f,
  .note_amplitude_minimum = 0.4f,
  .note_amplitude_maximum = 1.0f,
  .note_attack_minimum = 0.96f,
  .note_attack_maximum = 0.97f,
  .note_release_minimum = 0.96f,
  .note_release_maximum = 0.97f,
  .rand_source_type = rand_source_type_divisive,
  .allocated_scales = 0
};

void z_track_parameters_initialize(
  struct z_track_parameters* z_track_parameters
) {
  z_track_parameters_initialize_defaults(
    z_track_parameters
  );

  z_track_parameters->length_scales = 0;

  z_track_parameters->scales = malloc(
    sizeof(const unsigned char*) *
    z_track_parameters->length_scales
  );

  z_track_parameters->scales_length = malloc(
    sizeof(unsigned char) *
    z_track_parameters->length_scales
  );

  z_track_parameters->allocated_scales = 1;
}

void z_track_parameters_initialize_defaults(
  struct z_track_parameters* z_track_parameters
) {
  z_track_parameters->length_scales = (
    z_track_parameters_defaults.length_scales
  );
  
  z_track_parameters->scales = (
    z_track_parameters_defaults.scales
  );
  
  z_track_parameters->scales_length = (
    z_track_parameters_defaults.scales_length
  );
  
  z_track_parameters->length_scales = (
    z_track_parameters_defaults.length_scales
  );

  z_track_parameters->track_length_lanes_minimum = (
    z_track_parameters_defaults.track_length_lanes_minimum
  );

  z_track_parameters->track_length_lanes_maximum = (
    z_track_parameters_defaults.track_length_lanes_maximum
  );

  z_track_parameters->frequency_root = (
    z_track_parameters_defaults.frequency_root
  );

  z_track_parameters->octave_minimum = (
    z_track_parameters_defaults.octave_minimum
  );

  z_track_parameters->octave_maximum = (
    z_track_parameters_defaults.octave_maximum
  );

  for (
    unsigned char index_signal = 0;
    index_signal < z_track_parameters_length_signals_default;
    ++index_signal
  ) {
    z_track_parameters->signals[
      index_signal
    ] = (
      z_track_parameters_defaults.signals[
        index_signal
      ]
    );
  }

  z_track_parameters->track_length_multiplier = (
    z_track_parameters_defaults.track_length_multiplier
  );

  z_track_parameters->track_speed_multiplier = (
    z_track_parameters_defaults.track_speed_multiplier
  );

  z_track_parameters->oscillator_amplitude_minimum = (
    z_track_parameters_defaults.oscillator_amplitude_minimum
  );

  z_track_parameters->oscillator_amplitude_maximum = (
    z_track_parameters_defaults.oscillator_amplitude_maximum
  );

  z_track_parameters->note_amplitude_minimum = (
    z_track_parameters_defaults.note_amplitude_minimum
  );

  z_track_parameters->note_amplitude_maximum = (
    z_track_parameters_defaults.note_amplitude_maximum
  );

  z_track_parameters->note_attack_minimum = (
    z_track_parameters_defaults.note_attack_minimum
  );

  z_track_parameters->note_attack_maximum = (
    z_track_parameters_defaults.note_attack_maximum
  );

  z_track_parameters->note_release_minimum = (
    z_track_parameters_defaults.note_release_minimum
  );
  
  z_track_parameters->note_release_maximum = (
    z_track_parameters_defaults.note_release_maximum
  );

  z_track_parameters->rand_source_type = (
    z_track_parameters_defaults.rand_source_type
  );

  z_track_parameters->allocated_scales = (
    z_track_parameters_defaults.allocated_scales
  );
}

void z_track_parameters_destroy(
  struct z_track_parameters* z_track_parameters
) {
  if (
    z_track_parameters->allocated_scales != 0
  ) {
    free(
      z_track_parameters->scales
    );

    free(
      z_track_parameters->scales_length
    );
  }
}
