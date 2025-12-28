#include <z_track_parameters.h>

#include <cer0_frequency_root.h>
#include <cer0_scale.h>
#include <cer0_signal.h>

#include <rand_source_type.h>

#include <stdlib.h>

const unsigned char* z_track_scales_defaults[
  z_track_parameters_length_scales_default
] = {
  cer0_scale_notes_minor_pentatonic,
  cer0_scale_notes_harmonic_minor,
  cer0_scale_notes_melodic_minor_descending,
  cer0_scale_notes_octatonic_minor,
  cer0_scale_notes_neapolitan_minor
};

unsigned char z_track_scales_lengths_defaults[
  z_track_parameters_length_scales_default
] = {
  cer0_scale_length_minor_pentatonic,
  cer0_scale_length_harmonic_minor,
  cer0_scale_length_melodic_minor_descending,
  cer0_scale_length_octatonic_minor,
  cer0_scale_length_neapolitan_minor
};

const struct z_track_parameters z_track_parameters_defaults = {
  .scales = z_track_scales_defaults,
  .scales_length = z_track_scales_lengths_defaults,
  .length_scales = z_track_parameters_length_scales_default,
  .track_length_lanes_minimum = 4,
  .track_length_lanes_maximum = 8,
  .frequency_root = cer0_frequency_root_magic,
  .octave_minimum = 2,
  .octave_maximum = 5,
  .signals = {
    sine,
    sine,
    sine,
    triangle,
    triangle,
    sine
  },
  .track_length_multiplier = 6.0f,
  .track_speed_multiplier = 0.1f,
  .oscillator_amplitude_minimum = 0.5f,
  .oscillator_amplitude_maximum = 1.0f,
  .note_amplitude_minimum = 0.1f,
  .note_amplitude_maximum = 1.0f,
  .note_attack_minimum = 0.9f,
  .note_attack_maximum = 1.0f,
  .note_release_minimum = 0.9f,
  .note_release_maximum = 1.0f,
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
    z_track_parameters->signals[index_signal] = (
      z_track_parameters_defaults.signals[index_signal]
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
