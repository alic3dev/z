#include <z_track_parameters.h>

#include <cer0_attack_sustain_decay_release.h>
#include <cer0_frequency_root.h>
#include <cer0_scale.h>
#include <cer0_signal.h>

#include <clic3_bytes.h>
#include <clic3_memory.h>

const unsigned char* z_track_parameters_scales_defaults[
  z_track_parameters_length_scales_default
] = {
  cer0_scale_notes_major_pentatonic,
  cer0_scale_notes_harmonic_major,
  cer0_scale_notes_harmonic_major,
  cer0_scale_notes_harmonic_major,
  cer0_scale_notes_major_pentatonic
};

unsigned char z_track_parameters_scales_lengths_defaults[
  z_track_parameters_length_scales_default
] = {
  cer0_scale_length_major_pentatonic,
  cer0_scale_length_harmonic_major,
  cer0_scale_length_harmonic_major,
  cer0_scale_length_harmonic_major,
  cer0_scale_length_major_pentatonic
};

const unsigned char z_track_parameters_signals_defaults[
  z_track_parameters_length_signals_default
] = {
  sine,
  sine,
  sine,
  sine,
  sine,
  sine
};
const unsigned char z_track_parameters_types_defaults[
  z_track_parameters_length_types_default
] = {
  z_track_lane_type_bass,
  z_track_lane_type_rhythm_chords,
  z_track_lane_type_rhythm_notes,
  z_track_lane_type_chords,
  z_track_lane_type_notes
};

const struct z_track_parameters z_track_parameters_defaults = {
  .scales = (
    0x00
  ),
  .scales_length = (
    0x00
  ),
  .length_scales = (
    z_track_parameters_length_scales_default
  ),
  .track_length_lanes_minimum = (
    0x04
  ),
  .track_length_lanes_maximum = (
    0x04
  ),
  .frequency_root = (
    cer0_frequency_root_standard
  ),
  .octave_minimum = (
    0x01
  ),
  .octave_maximum = (
    0x06
  ),
  .signals = (
    0x00
  ),
  .length_signals = (
    z_track_parameters_length_signals_default
  ),
  .types = (
    0x00
  ),
  .length_types = (
    z_track_parameters_length_types_default
  ),
  .track_length_multiplier = (
    8.0f
  ),
  .track_bpm_minimum = (
    0x4a
  ),
  .track_bpm_maximum = (
    0x7a
  ),
  .oscillator_amplitude_minimum = (
    0.825f
  ),
  .oscillator_amplitude_maximum = (
    1.0f
  ),
  .note_amplitude_minimum = (
    0.125f
  ),
  .note_amplitude_maximum = (
    1.0f
  )
};

void z_track_parameters_initialize(
  struct z_track_parameters* z_track_parameters
) {
  z_track_parameters_initialize_defaults(
    z_track_parameters
  );
}

void z_track_parameters_initialize_defaults(
  struct z_track_parameters* z_track_parameters
) {
  clic3_bytes_copy(
    z_track_parameters,
    (
      (void*)
      &z_track_parameters_defaults
    ),
    sizeof(
      struct z_track_parameters
    )
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
  
  z_track_parameters->signals = (
    clic3_memory_allocate_raw(
      z_track_parameters->length_signals
    )
  );
  
  z_track_parameters->types = (
    clic3_memory_allocate_raw(
      z_track_parameters->length_types
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
      z_track_parameters_scales_defaults[
        index_scale %
        z_track_parameters_length_scales_default
      ]
    );

    z_track_parameters->scales_length[
      index_scale
    ] = (
      z_track_parameters_scales_lengths_defaults[
        index_scale %
        z_track_parameters_length_scales_default
      ]
    );
  }

  for (
    unsigned char index_signal = (
      0x00
    );
    (
      index_signal <
      z_track_parameters->length_signals
    );
    ++index_signal
  ) {
    z_track_parameters->signals[
      index_signal
    ] = (
      z_track_parameters_signals_defaults[
        index_signal %
        z_track_parameters_length_signals_default
      ]
    );
  }
    for (
    unsigned char index_type = (
      0x00
    );
    (
      index_type <
      z_track_parameters->length_types
    );
    ++index_type
  ) {
    z_track_parameters->types[
      index_type
    ] = (
      z_track_parameters_types_defaults[
        index_type %
        z_track_lane_types_length
      ]
    );
  }

  cer0_attack_sustain_decay_release_parameters_initialize(
    &z_track_parameters->attack_sustain_decay_release_parameters_minimum
  );

  cer0_attack_sustain_decay_release_parameters_initialize(
    &z_track_parameters->attack_sustain_decay_release_parameters_maximum
  );

  z_track_parameters->attack_sustain_decay_release_parameters_minimum.attack = (
    0.0f
  );

  z_track_parameters->attack_sustain_decay_release_parameters_maximum.attack = (
    0.0f
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
  
  clic3_memory_free_raw(
    z_track_parameters->signals
  );
  
  clic3_memory_free_raw(
    z_track_parameters->types
  );
}
