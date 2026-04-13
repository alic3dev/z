#include <z_track_parameters.h>

#include <cer0_frequency_root.h>
#include <cer0_scale.h>
#include <cer0_signal.h>

#include <clic3_memory.h>

#include <rand_source_type.h>

const unsigned char* z_track_scales_defaults[
  z_track_parameters_length_scales_default
] = {
  cer0_scale_notes_major_pentatonic,
  cer0_scale_notes_harmonic_minor,
  cer0_scale_notes_major_pentatonic,
  cer0_scale_notes_harmonic_minor,
  cer0_scale_notes_neapolitan_major
};

unsigned char z_track_scales_lengths_defaults[
  z_track_parameters_length_scales_default
] = {
  cer0_scale_length_major_pentatonic,
  cer0_scale_length_harmonic_minor,
  cer0_scale_length_major_pentatonic,
  cer0_scale_length_harmonic_minor,
  cer0_scale_length_neapolitan_major
};

const struct z_track_parameters z_track_parameters_defaults = {
  .scales = z_track_scales_defaults,
  .scales_length = z_track_scales_lengths_defaults,
  .length_scales = z_track_parameters_length_scales_default,
  .track_length_lanes_minimum = 6,
  .track_length_lanes_maximum = 8,
  .frequency_root = cer0_frequency_root_standard,
  .octave_minimum = 0,
  .octave_maximum = 10,
  .signals = {
    square,
    sine,
    triangle,
    sine,
    square,
    sawtooth_down
  },
  .track_length_multiplier = 3.15f,
  .track_bpm_minimum = 50.0f,
  .track_bpm_maximum = 60.0f,
  .oscillator_amplitude_minimum = 0.825f,
  .oscillator_amplitude_maximum = 0.9f,
  .note_amplitude_minimum = 0.125f,
  .note_amplitude_maximum = 0.9f,
  .note_attack_minimum = 0.0f,
  .note_attack_maximum = 0.0f,
  .note_release_minimum = 0.0f,
  .note_release_maximum = 0.0f,
  .rand_source_type = rand_source_type_divisive,
  .allocated_scales = 0
};

void z_track_parameters_initialize(
  struct z_track_parameters* z_track_parameters
) {
  z_track_parameters_initialize_defaults(
    z_track_parameters
  );

  z_track_parameters->length_scales = (
    0
  );

  z_track_parameters->scales = (
    clic3_memory_allocate_raw(
      sizeof(
        void*
      ) *
      z_track_parameters->length_scales
    )
  );

  z_track_parameters->scales_length = (
    clic3_memory_allocate_raw(
      z_track_parameters->length_scales
    )
  );

  z_track_parameters->allocated_scales = (
    1
  );
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

  z_track_parameters->track_bpm_minimum = (
    z_track_parameters_defaults.track_bpm_minimum
  );

  z_track_parameters->track_bpm_maximum = (
    z_track_parameters_defaults.track_bpm_maximum
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
    clic3_memory_free_raw(
      z_track_parameters->scales
    );

    clic3_memory_free_raw(
      z_track_parameters->scales_length
    );
  }
}
