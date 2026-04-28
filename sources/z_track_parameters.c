#include <z_track_parameters.h>

#include <cer0_attack_sustain_decay_release.h>
#include <cer0_frequency_root.h>
#include <cer0_scale.h>
#include <cer0_signal.h>

#include <clic3_memory.h>

const unsigned char* z_track_scales_defaults[
  z_track_parameters_length_scales_default
] = {
  cer0_scale_notes_major_pentatonic,
  cer0_scale_notes_harmonic_major,
  cer0_scale_notes_harmonic_major,
  cer0_scale_notes_harmonic_major,
  cer0_scale_notes_major_pentatonic
};

unsigned char z_track_scales_lengths_defaults[
  z_track_parameters_length_scales_default
] = {
  cer0_scale_length_major_pentatonic,
  cer0_scale_length_harmonic_major,
  cer0_scale_length_harmonic_major,
  cer0_scale_length_harmonic_major,
  cer0_scale_length_major_pentatonic
};

const struct z_track_parameters z_track_parameters_defaults = {
  .scales = (
    0x00
  ),
  .scales_length = (
    z_track_scales_lengths_defaults
  ),
  .length_scales = (
    z_track_parameters_length_scales_default
  ),
  .track_length_lanes_minimum = (
    0x06
  ),
  .track_length_lanes_maximum = (
    0x0a
  ),
  .frequency_root = (
    cer0_frequency_root_scientific
  ),
  .octave_minimum = (
    0x01
  ),
  .octave_maximum = (
    0x0a
  ),
  .signals = {
    sine,
    square,
    triangle,
    sawtooth_up,
    sawtooth_down,
    sine
  },
  .track_length_multiplier = (
    8.15f
  ),
  .track_bpm_minimum = (
    0x14
  ),
  .track_bpm_maximum = (
    0xc8
  ),
  .oscillator_amplitude_minimum = (
    0.825f
  ),
  .oscillator_amplitude_maximum = (
    0.9f
  ),
  .note_amplitude_minimum = (
    0.125f
  ),
  .note_amplitude_maximum = (
    0.9f
  )
};

void z_track_parameters_initialize(
  struct z_track_parameters* z_track_parameters
) {
  z_track_parameters_initialize_defaults(
    z_track_parameters
  );

  z_track_parameters->length_scales = (
    0x00
  );

  clic3_memory_reallocate_raw(
    &z_track_parameters->scales,
    (
      sizeof(
        void*
      ) *
      z_track_parameters->length_scales
    )
  );

  clic3_memory_reallocate_raw(
    &z_track_parameters->scales_length,
    z_track_parameters->length_scales
  );
}

void z_track_parameters_initialize_defaults(
  struct z_track_parameters* z_track_parameters
) {
  z_track_parameters->length_scales = (
    z_track_parameters_defaults.length_scales
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

  for (
    unsigned char index_scale = (
      0x00
    );
    (
      index_scale <
      z_track_parameters->length_scales
    );
    ++index_scale
  ) {
    z_track_parameters->scales[
      index_scale
    ] = (
      z_track_scales_defaults[
        index_scale
      ]
    );

    z_track_parameters->scales_length[
      index_scale
    ] = (
      z_track_scales_lengths_defaults[
        index_scale
      ]
    );
  }

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
    unsigned char index_signal = (
      0x00
    );
    (
      index_signal <
      z_track_parameters_length_signals_default
    );
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

  cer0_attack_sustain_decay_release_parameters_initialize(
    &z_track_parameters->attack_sustain_decay_release_parameters_minimum
  );

  cer0_attack_sustain_decay_release_parameters_initialize(
    &z_track_parameters->attack_sustain_decay_release_parameters_maximum
  );
}

void z_track_parameters_destroy(
  struct z_track_parameters* z_track_parameters
) {
  clic3_memory_free_raw(
    z_track_parameters->scales
  );

  clic3_memory_free_raw(
    z_track_parameters->scales_length
  );
}
