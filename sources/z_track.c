#include <z_track.h>

#include <z_queue.h>
#include <z_track_note.h>
#include <z_track_lane.h>
#include <z_words.h>

#include <clic3_base_hexadecimal.h>
#include <clic3_bytes.h>
#include <clic3_memory.h>

#include <cer0_attack_sustain_decay_release.h>
#include <cer0_effect.h>
#include <cer0_effects/cer0_effect_delay.h>
#include <cer0_effects/cer0_effect_distortion.h>
#include <cer0_octave_range.h>
#include <cer0_note_table.h>
#include <cer0_synthesizer.h>

#include <rand_clean.h>
#include <rand_initialize.h>
#include <rand_functions.h>
#include <rand_result.h>
#include <rand_source.h>

void z_track_generate(
  struct z_track* track,
  struct z_track_parameters* z_track_parameters,
  float sample_rate
) {
  rand_initialize(
    &track->rand_parameters,
    &track->rand_result,
    &track->rand_source,
    0xff,
    rand_mode_bytes,
    z_track_parameters->rand_source_type
  );

  unsigned char status_rand_get = (
    rand_get(
      &track->rand_source,
      &track->rand_result,
      &track->rand_parameters
    )
  );

  unsigned char size_float = (
    sizeof(
      float
    )
  );

  rand_get(
    &track->rand_source,
    &track->rand_result,
    &track->rand_parameters
  );

  unsigned char length_track_name = (
    (
      track->rand_result.bytes[
        0x00
      ] %
      0x06
    ) +
    0x02
  );

  track->name = (
    clic3_memory_allocate_raw(
      length_track_name
    )
  );

  struct rand_source_divisive_data* rand_source_divisive_data = (
    track->rand_source.data
  );

  track->length_char_array_seed = (
    rand_source_divisive_data->length_seed +
    rand_source_divisive_data->length_seed
  );

  track->char_array_seed = (
    clic3_memory_allocate_raw(
      track->length_char_array_seed +
      0x01
    )
  );

  for (
    unsigned int index_seed = 0;
    index_seed < rand_source_divisive_data->length_seed;
    ++index_seed
  ) {
    const char* char_seed_part = (
      clic3_base_hexadecimal_unsigned_char_mapping[
        rand_source_divisive_data->seed[
          index_seed
        ]
      ]
    );

    track->char_array_seed[
      index_seed *
      2
    ] = (
      char_seed_part[
        0
      ]
    );

    track->char_array_seed[
      index_seed *
      2 +
      1
    ] = (
      char_seed_part[
        1
      ]
    );
  }

  track->char_array_seed[
    track->length_char_array_seed
  ] = '\0';

  track->note_table = (
    cer0_note_table_create(
      z_track_parameters->octave_minimum,
      z_track_parameters->octave_maximum,
      z_track_parameters->frequency_root
    )
  );

  track->range_octave = cer0_octave_range_get(
    z_track_parameters->octave_minimum,
    z_track_parameters->octave_maximum
  );

  unsigned int range_octave_lower_minimum = (
    z_track_parameters->octave_minimum
  );

  unsigned int range_octave_lower_maximum = (
    (
      range_octave_lower_minimum +
      1
    ) >
    z_track_parameters->octave_maximum
    ? range_octave_lower_minimum
    : (
      range_octave_lower_minimum +
      1
    )
  );

  unsigned int range_octave_mid_minimum = (
    range_octave_lower_maximum
  );

  unsigned int range_octave_mid_maximum = (
    (
      range_octave_mid_minimum +
      2
    ) >
    z_track_parameters->octave_maximum
    ? (
      (
        range_octave_mid_minimum +
        1
      ) >
      z_track_parameters->octave_maximum
      ? range_octave_mid_minimum
      : (
        range_octave_mid_minimum +
        1
      )
    )
    : (
      range_octave_mid_minimum +
      2
    )
  );

  unsigned int range_octave_upper_minimum = (
    (
      range_octave_mid_maximum -
      1
    ) <
    z_track_parameters->octave_minimum
    ? range_octave_mid_maximum
    : (
      range_octave_mid_maximum -
      1
    )
  );

  unsigned int range_octave_upper_maximum = (
    z_track_parameters->octave_maximum
  );

  unsigned int range_octave_lower = (
    cer0_octave_range_get(
      range_octave_lower_minimum,
      range_octave_lower_maximum
    )
  );

  unsigned int range_octave_mid = (
    cer0_octave_range_get(
      range_octave_mid_minimum,
      range_octave_mid_maximum
    )
  );

  unsigned int range_octave_upper = (
    cer0_octave_range_get(
      range_octave_upper_minimum,
      range_octave_upper_maximum
    )
  );

  track->length_note_table = (
    cer0_note_table_length(
      z_track_parameters->octave_minimum,
      z_track_parameters->octave_maximum
    )
  );

  track->progress = (
    0x00
  );

  unsigned char index_scale = (
    track->rand_result.bytes[
      3
    ] %
    z_track_parameters->length_scales
  );

  track->scale = (
    z_track_parameters->scales[
      index_scale
    ]
  );

  track->length_scale = (
    z_track_parameters->scales_length[
      index_scale
    ]
  );

  track->key = (
    track->rand_result.bytes[
      4
    ] %
    cer0_default_steps_notes
  );

  rand_get(
    &track->rand_source,
    &track->rand_result,
    &track->rand_parameters
  );

  for (
    unsigned char index_track_name = (
      0x00
    );
    (
      index_track_name <
      (
        length_track_name -
        0x01
      )
    );
    ++index_track_name
  ) {
    track->name[
      index_track_name
    ] = (
      track->rand_result.bytes[
        index_track_name
      ] %
      0x7e +
      0x01
    );

    for (
      unsigned char index_track_name_lookup = (
        0x00
      );
      (
        index_track_name_lookup <
        index_track_name
      );
    ) {
      if (
        track->name[
          index_track_name
        ] ==
        track->name[
          index_track_name_lookup
        ]
      ) {
        track->name[
          index_track_name
        ] = (
          (
            track->name[
              index_track_name
            ] +
            0x01
          ) %
          0x7e +
          0x01
        );

        index_track_name_lookup = (
          0x00
        );
      } else {
        index_track_name_lookup = (
          index_track_name_lookup +
          0x01
        );
      }
    }
  }

  track->name[
    length_track_name -
    0x01
  ] = (
    '\0'
  );

  rand_get(
    &track->rand_source,
    &track->rand_result,
    &track->rand_parameters
  );

  if (
    z_track_parameters->track_bpm_minimum >
    z_track_parameters->track_bpm_maximum
  ) {
    float track_bpm_swap = (
      z_track_parameters->track_bpm_minimum
    );

    z_track_parameters->track_bpm_minimum = (
      z_track_parameters->track_bpm_maximum
    );

    z_track_parameters->track_bpm_maximum = (
      track_bpm_swap
    );
  }

  float track_bpm_range = (
    z_track_parameters->track_bpm_maximum -
    z_track_parameters->track_bpm_minimum
  );

  track->bpm = (
    z_track_parameters->track_bpm_minimum +
    (float)
    (
      (
        track->rand_result.bytes[
          0x00
        ] +
        0x01
      ) + (
        track->rand_result.bytes[
          0x01
        ] +
        0x01
      )
    ) /
    0x01fe *
    track_bpm_range
  );

  float whole_beat = (
    (
      0x3c /
      track->bpm
    ) *
    1000.0f
  );

  float half_beat = (
    whole_beat /
    0x02
  );

  float quarter_beat = (
    whole_beat /
    0x04
  );

  float eigth_beat = (
    whole_beat /
    0x08
  );

  float sixtenth_beat = (
    whole_beat /
    0x10
  );

  track->length = (
    (float) (
      track->rand_result.bytes[2] +
      track->rand_result.bytes[3]
    ) *
    0x20 *
    z_track_parameters->track_length_multiplier
  );

  track->length_lanes = (
    (
      (float)
      track->rand_result.bytes[
        0x04
      ] /
      0xff *
      (
        z_track_parameters->track_length_lanes_maximum -
        z_track_parameters->track_length_lanes_minimum
      ) +
      z_track_parameters->track_length_lanes_minimum
    )
  );

  track->lanes = (
    clic3_memory_allocate_raw(
      sizeof(
        struct z_track_lane
      ) *
      track->length_lanes
    )
  );

  for (
    unsigned int index_lane = (
      0x00
    );
    (
      index_lane <
      track->length_lanes
    );
    ++index_lane
  ) {
    struct z_track_lane* track_lane = &(
      track->lanes[
        index_lane
      ]
    );

    rand_get(
      &track->rand_source,
      &track->rand_result,
      &track->rand_parameters
    );

    cer0_synthesizer_initialize(
      &track_lane->synthesizer,
      sample_rate
    );

    static struct cer0_effect* effect_delay;
    static struct cer0_effect* effect_delay_second;
    static struct cer0_effect* effect_distortion;

    effect_delay = (
      clic3_memory_allocate_raw(
        sizeof(
          struct cer0_effect
        )
      )
    );

    effect_delay_second = (
      clic3_memory_allocate_raw(
        sizeof(
          struct cer0_effect
        )
      )
    );

    effect_distortion = (
      clic3_memory_allocate_raw(
        sizeof(
          struct cer0_effect
        )
      )
    );

    cer0_effect_delay_initialize(
      effect_delay
    );

    cer0_effect_delay_initialize(
      effect_delay_second
    );
  
    cer0_effect_distortion_initialize(
      effect_distortion
    );

    effect_delay->mix = (
      0.45f
    );

    effect_delay_second->mix = (
      0.5f
    );

    cer0_effect_delay_length_frames_buffer_set(
      effect_delay_second,
      0xffff
    );
    
    struct cer0_effect_delay_data* effect_delay_data = (
      effect_delay->data
    );

    struct cer0_effect_delay_data* effect_delay_data_second = (
      effect_delay_second->data
    );

    effect_delay_data_second->decay = (
      0.125f
    );
  
    struct cer0_effect_distortion_data* effect_distortion_data = (
      effect_distortion->data
    );

    cer0_effect_delay_length_frames_buffer_set(
      effect_delay,
      0xfffff
    );

    effect_delay_data->decay = (
      0.125f
    );

    effect_distortion_data->gain = (
      2.345f
    );
    
    effect_distortion_data->noise = (
      0x00
    );

    effect_distortion->mix = (
      0x00
    );

    cer0_synthesizer_effect_add(
      &track_lane->synthesizer,
      effect_delay
    );

    cer0_synthesizer_effect_add(
      &track_lane->synthesizer,
      effect_delay_second
    );

    cer0_synthesizer_effect_add(
      &track_lane->synthesizer,
      effect_distortion
    );

    unsigned char count_oscillators = (
      track->rand_result.bytes[
        0x00
      ] %
      0x04 +
      0x01
    );

    for (
      unsigned char index_oscillator = 0;
      index_oscillator < count_oscillators;
      ++index_oscillator
    ) {
      enum cer0_signal signal;

      if (
        index_lane ==
        0x00
      ) {
        signal = (
          (
            track->rand_result.bytes[
              0x01
            ] <=
            0xc8
          )
          ? z_track_parameters->signals[
            0x00
          ]
          : (
            (
              track->rand_result.bytes[
                0x01
              ] <=
              0xe6
            )
            ? (
              z_track_parameters->signals[
                z_track_parameters_length_signals_default -
                0x02
              ]
            )
            : (
              z_track_parameters->signals[
                z_track_parameters_length_signals_default -
                0x01
              ]
            )
          )
        );
      } else {
        signal = (
          z_track_parameters->signals[
            track->rand_result.bytes[
              0x01
            ] %
            z_track_parameters_length_signals_default
          ]
        );
      }

      cer0_synthesizer_oscillator_add(
        &track_lane->synthesizer,
        signal
      );

      track_lane->synthesizer.oscillators[
        index_oscillator
      ].amplitude = (
        (float) (
          track->rand_result.bytes[
            0x02
          ]
        ) /
        0xff *
        (
          z_track_parameters->oscillator_amplitude_maximum -
          z_track_parameters->oscillator_amplitude_minimum
        ) +
        z_track_parameters->oscillator_amplitude_minimum
      );
    }

    track_lane->length_notes = (
      (
        (
          track->rand_result.bytes[
            0x04
          ] %
          0x10
        ) +
        0x08
      ) *
      0x40
    );

    track_lane->notes = (
      clic3_memory_allocate_raw(
        sizeof(
          struct z_track_note
        ) *
        track_lane->length_notes
      )
    );

    rand_get(
      &track->rand_source,
      &track->rand_result,
      &track->rand_parameters
    );

    unsigned char speed = (
      (
        index_lane == 
        0x00
      )
      ? 0x00
      : (
        (
          index_lane
        ) %
        0x03 +
        0x01
      )
    );

    struct z_track_note* notes = (
      track_lane->notes
    );

    for (
      unsigned long int index_note = (
        0x00
      );
      (
        index_note <
        track_lane->length_notes
      );
      ++index_note
    ) {
      rand_get(
        &track->rand_source,
        &track->rand_result,
        &track->rand_parameters
      );

      struct z_track_note* note = &(
        notes[
          index_note
        ]
      );

      note->amplitude = (
        (float)
        track->rand_result.bytes[
          0x05
        ] /
        0xff *
        (
          z_track_parameters->note_amplitude_maximum -
          z_track_parameters->note_amplitude_minimum
        ) +
        z_track_parameters->note_amplitude_minimum
      );

      cer0_attack_sustain_decay_release_parameters_initialize(
        &note->attack_sustain_decay_release_parameters
      );

      note->attack_sustain_decay_release_parameters.attack = (
        (
          (float)
          track->rand_result.bytes[
            0x06
          ] /
          0xff
        ) *
        (
          z_track_parameters->attack_sustain_decay_release_parameters_maximum.attack -
          z_track_parameters->attack_sustain_decay_release_parameters_minimum.attack
        ) +
        z_track_parameters->attack_sustain_decay_release_parameters_minimum.attack
      );

      note->attack_sustain_decay_release_parameters.sustain = (
        (
          (float)
          track->rand_result.bytes[
            0x07
          ] /
          0xff
        ) *
        (
          z_track_parameters->attack_sustain_decay_release_parameters_maximum.sustain -
          z_track_parameters->attack_sustain_decay_release_parameters_minimum.sustain
        ) +
        z_track_parameters->attack_sustain_decay_release_parameters_minimum.sustain
      );

      note->attack_sustain_decay_release_parameters.decay = (
        (
          (float)
          track->rand_result.bytes[
            0x08
          ] /
          0xff
        ) *
        (
          z_track_parameters->attack_sustain_decay_release_parameters_maximum.decay -
          z_track_parameters->attack_sustain_decay_release_parameters_minimum.decay
        ) +
        z_track_parameters->attack_sustain_decay_release_parameters_minimum.decay
      );

      note->attack_sustain_decay_release_parameters.release = (
        (
          (float)
          track->rand_result.bytes[
            0x09
          ] /
          0xff
        ) *
        (
          z_track_parameters->attack_sustain_decay_release_parameters_maximum.release -
          z_track_parameters->attack_sustain_decay_release_parameters_minimum.release
        ) +
        z_track_parameters->attack_sustain_decay_release_parameters_minimum.release
      );

      note->attack_sustain_decay_release_parameters.amplitude_initial = (
        (
          (float)
          track->rand_result.bytes[
            0x0a
          ] /
          0xff
        ) *
        (
          z_track_parameters->attack_sustain_decay_release_parameters_maximum.amplitude_initial -
          z_track_parameters->attack_sustain_decay_release_parameters_minimum.amplitude_initial
        ) +
        z_track_parameters->attack_sustain_decay_release_parameters_minimum.amplitude_initial
      );

      note->attack_sustain_decay_release_parameters.amplitude_attack = (
        (
          (float)
          track->rand_result.bytes[
            0x0b
          ] /
          0xff
        ) *
        (
          z_track_parameters->attack_sustain_decay_release_parameters_maximum.amplitude_attack -
          z_track_parameters->attack_sustain_decay_release_parameters_minimum.amplitude_attack
        ) +
        z_track_parameters->attack_sustain_decay_release_parameters_minimum.amplitude_attack
      );

      note->attack_sustain_decay_release_parameters.amplitude_sustain = (
        (
          (float)
          track->rand_result.bytes[
            0x0c
          ] /
          0xff
        ) *
        (
          z_track_parameters->attack_sustain_decay_release_parameters_maximum.amplitude_sustain -
          z_track_parameters->attack_sustain_decay_release_parameters_minimum.amplitude_sustain
        ) +
        z_track_parameters->attack_sustain_decay_release_parameters_minimum.amplitude_sustain
      );
      
      note->attack_sustain_decay_release_parameters.amplitude_decay = (

        (
          (float)
          track->rand_result.bytes[
            0x0d
          ] /
          0xff
        ) *
        (
          z_track_parameters->attack_sustain_decay_release_parameters_maximum.amplitude_decay -
          z_track_parameters->attack_sustain_decay_release_parameters_minimum.amplitude_decay
        ) +
        z_track_parameters->attack_sustain_decay_release_parameters_minimum.amplitude_decay
      );
      
      note->attack_sustain_decay_release_parameters.amplitude_release = (
        (
          (float)
          track->rand_result.bytes[
            0x0e
          ] /
          0xff
        ) *
        (
          z_track_parameters->attack_sustain_decay_release_parameters_maximum.amplitude_release -
          z_track_parameters->attack_sustain_decay_release_parameters_minimum.amplitude_release
        ) +
        z_track_parameters->attack_sustain_decay_release_parameters_minimum.amplitude_release
      );

      float length_note;

      switch (
        speed
      ) {
        case 0: {
          length_note = (
            (float)
            (
              (
                track->rand_result.bytes[
                  3
                ] % 5
              ) +
              4
            ) *
            half_beat
          );

          break;
        }
        case 1: {
          length_note = (
            (float)
            (
              (
                track->rand_result.bytes[
                  3
                ] % 9
              ) +
              2
            ) *
            quarter_beat
          );

          break;
        }
        case 2: {
          length_note = (
            (float)
            (
              (
                track->rand_result.bytes[
                  3
                ] % 15
              ) +
              2
            ) *
            eigth_beat
          );

          break;
        }
        case 3: {
          length_note = (
            (float)
            (
              (
                track->rand_result.bytes[
                  3
                ] % 32
              ) +
              3
            ) *
            sixtenth_beat
          );

          break;
        }
      }

      length_note = (
        length_note *
        (index_lane > 6 ? 0.2f : 1.0f)
      );

      if (
        index_note > 0
      ) {
        note->time = (
          notes[
            index_note -
            1
          ].time +
          length_note
        );
      } else {
        note->time = (
          length_note
        );
      }

      unsigned char position_scale = (
        track->scale[
          (
            (
              track->rand_result.bytes[2] +
              1
            )
          ) %
          track->length_scale
        ]
      );

      float value_note;

      if (
        index_lane > 1 &&
        index_note >= 32
      ) {
        value_note = (
          notes[
            (
              index_note %
              8
            ) %
            track_lane->length_notes
          ].value
        );
      } else {
        unsigned int octave_range_minimum;
        unsigned int octave_range;

        if (
          index_lane == 0
        ) {
          octave_range_minimum = (
            range_octave_lower_minimum
          );

          octave_range = (
            range_octave_lower
          );
        } else if (
          (
            index_lane %
            2
          ) == 0
        ) {
          octave_range_minimum = (
            range_octave_mid_minimum
          );

          octave_range = (
            range_octave_mid
          );
        } else {
          octave_range_minimum = (
            range_octave_upper_minimum
          );

          octave_range = (
            range_octave_upper
          );
        }

        unsigned int position_note_table = (
          (
            (
              position_scale +
              track->key
            ) +
            (
              cer0_default_steps_notes *
              (
                track->rand_result.bytes[
                  4
                ] %
                octave_range
              )
            )
          ) %
          (
            octave_range *
            cer0_default_steps_notes
          ) +
          (
            octave_range_minimum *
            cer0_default_steps_notes
          )
        );

        value_note = (
          track->note_table[
            position_note_table %
            track->length_note_table
          ]
        );
      }

      note->value = (
        value_note
      );
    }

    clic3_bytes_copy(
      &track_lane->synthesizer.attack_sustain_decay_release_parameters,
      &notes[
        0x00
      ].attack_sustain_decay_release_parameters,
      sizeof(
        struct cer0_attack_sustain_decay_release_parameters
      )
    );

    track_lane->synthesizer.length_attack_sustain_decay_release = (
      notes[
        0x00
      ].time *
      sample_rate /
      600.0f
    );

    cer0_synthesizer_frequency_play(
      &track_lane->synthesizer,
      notes[
        0x00
      ].value
    );
  }
}

void z_track_destroy(
  struct z_track* track
) {
  clic3_memory_free_raw(
    track->name
  );

  clic3_memory_free_raw(
    track->note_table
  );

  clic3_memory_free_raw(
    track->char_array_seed
  );

  rand_clean(
    &track->rand_result,
    &track->rand_source
  );

  for (
    unsigned int index_lane = 0;
    index_lane < track->length_lanes;
    ++index_lane
  ) {
    cer0_synthesizer_destroy(
      &track->lanes[
        index_lane
      ].synthesizer
    );

    clic3_memory_free_raw(
      track->lanes[
        index_lane
      ].notes
    );
  }

  clic3_memory_free_raw(
    track->lanes
  );
}
