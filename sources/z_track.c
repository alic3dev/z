#include <z_track.h>

#include <z_queue.h>
#include <z_track_note.h>
#include <z_track_lane.h>
#include <z_track_lane_type.h>
#include <z_words.h>

#include <clic3_base_hexadecimal.h>
#include <clic3_bytes.h>
#include <clic3_memory.h>

#include <cer0_attack_sustain_decay_release.h>
#include <cer0_effect.h>
#include <cer0_effects/cer0_effect_bit_crush.h>
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
#include <rand_source_type.h>

void z_track_generate(
  struct z_track* track,
  struct z_track_parameters* z_track_parameters,
  float sample_rate
) {
  rand_initialize(
    &track->rand_parameters,
    &track->rand_result,
    &track->rand_source,
    0x1f,
    rand_mode_bytes,
    rand_source_type_divisive
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
    rand_source_divisive_data->length_seed *
    0x02
  );

  track->char_array_seed = (
    clic3_memory_allocate_raw(
      track->length_char_array_seed +
      0x01
    )
  );

  for (
    unsigned int index_seed = (
      0x00
    );
    (
      index_seed <
      rand_source_divisive_data->length_seed
    );
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
      0x02
    ] = (
      char_seed_part[
        0x00
      ]
    );

    track->char_array_seed[
      index_seed *
      0x02 +
      0x01
    ] = (
      char_seed_part[
        0x01
      ]
    );
  }

  track->char_array_seed[
    track->length_char_array_seed
  ] = (
    '\0'
  );

  track->note_table = (
    cer0_note_table_create(
      z_track_parameters->octave_minimum,
      z_track_parameters->octave_maximum,
      z_track_parameters->frequency_root
    )
  );

  track->range_octave = (
    cer0_octave_range_get(
      z_track_parameters->octave_minimum,
      z_track_parameters->octave_maximum
    )
  );

  unsigned int range_octave_lower_minimum = (
    z_track_parameters->octave_minimum
  );

  unsigned int range_octave_lower_maximum = (
    (
      (
        range_octave_lower_minimum +
        0x01
      ) >
      z_track_parameters->octave_maximum
    )
    ? range_octave_lower_minimum
    : (
      range_octave_lower_minimum +
      0x01
    )
  );

  unsigned int range_octave_mid_minimum = (
    range_octave_lower_maximum
  );

  unsigned int range_octave_mid_maximum = (
    (
      (
        range_octave_mid_minimum +
        0x02
      ) >
      z_track_parameters->octave_maximum
    )
    ? (
      (
        range_octave_mid_minimum +
        0x01
      ) >
      z_track_parameters->octave_maximum
      ? range_octave_mid_minimum
      : (
        range_octave_mid_minimum +
        0x01
      )
    )
    : (
      range_octave_mid_minimum +
      0x02
    )
  );

  unsigned int range_octave_upper_minimum = (
    (
      (
        range_octave_mid_maximum -
        0x01
      ) <
      z_track_parameters->octave_minimum
    )
    ? range_octave_mid_maximum
    : (
      range_octave_mid_maximum -
      0x01
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
      0x03
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
      0x04
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
    0x3c /
    track->bpm *
    0x03e8
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

  track->length_effects = (
    0x03
  );

  track->effects = (
    clic3_memory_allocate_raw(
      sizeof(
        struct cer0_effect
      ) *
      track->length_effects
    )
  );

  for (
    unsigned char index_delay = (
      0x00
    );
    (
      index_delay <
      0x03
    );
    ++index_delay
  ) {
    cer0_effect_delay_initialize(
      &track->effects[
        index_delay
      ],
      0x02
    );

    track->effects[
       index_delay
    ].mix = (
      0.3f +
      (
        (float)
        (
          0x02 -
          index_delay
        ) *
        0.05f
      )
    );

    cer0_effect_delay_length_frames_buffer_set(
      &track->effects[
        index_delay
      ],
      (
        sample_rate /
        0x0258
      ) *
      quarter_beat *
      (float)
      (
        index_delay +
        0x01
      )
    );

    struct cer0_effect_delay_data* effect_delay_data = (
      track->effects[
         index_delay
      ].data
    );

    effect_delay_data->decay = (
      0.4f /
      (float)
      (
        index_delay +
        0x01
      )
    );
  }

  track->length = (
    (float)
    (
      track->rand_result.bytes[
        0x02
      ] +
      track->rand_result.bytes[
        0x03
      ]
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

    if (
      index_lane ==
      0x00
    ) {
      track_lane->type = (
        z_track_parameters->types[
          0x00
        ]
      );
    } else if (
      index_lane ==
      0x01
    ) {
      track_lane->type = (
        z_track_parameters->types[
          0x01
        ]
      );
    } else {
      track_lane->type = (
        z_track_parameters->types[
         (
           (
             track->rand_result.bytes[
               0x00
             ] %
             (
               z_track_parameters->length_types -
               0x02
             )
           ) +
           0x02
         ) %
         z_track_parameters->length_types
       ]
     );
    }

    cer0_synthesizer_initialize(
      &track_lane->synthesizer,
      sample_rate
    );

    if (
      (
        track_lane->type ==
        z_track_lane_type_notes
      ) ||
      (
        track_lane->type ==
        z_track_lane_type_rhythm_notes
      )
    ) {
      if (
        track->rand_result.bytes[
          0x10
        ] >
        0x80
      ) {
        struct cer0_effect* effect_delay = (
          cer0_synthesizer_effect_add(
            &track_lane->synthesizer
          )
        );

        cer0_effect_delay_initialize(
          effect_delay,
          0x02
        );

        cer0_effect_delay_length_frames_buffer_set(
          effect_delay,
          (
            sixtenth_beat *
            0.4f *
            (
              sample_rate /
              0x0258
            )
          )
        );

        struct cer0_effect_delay_data* effect_delay_data = (
          effect_delay->data
        );

        effect_delay_data->decay = (
          (float)
          track->rand_result.bytes[
            0x15
          ] /
          0xff *
          0.2f +
          0.1f
        );

        effect_delay->mix = (
          0.5f
        );
      }

      if (
        track->rand_result.bytes[
          0x12
        ] >
        0x20
      ) {
        struct cer0_effect* effect_delay = (
          cer0_synthesizer_effect_add(
            &track_lane->synthesizer
          )
        );

        cer0_effect_delay_initialize(
          effect_delay,
          0x02
        );

        cer0_effect_delay_length_frames_buffer_set(
          effect_delay,
          (
            sixtenth_beat *
            2.9f *
            (
              sample_rate /
              0x0258
            )
          )
        );

        struct cer0_effect_delay_data* effect_delay_data = (
          effect_delay->data
        );

        effect_delay_data->decay = (
          (float)
          track->rand_result.bytes[
            0x15
          ] /
          0xff *
          0.2f +
          0.1f
        );
      }
    }

    unsigned char count_oscillators = (
      track->rand_result.bytes[
        0x00
      ] %
      0x06 +
      0x01
    );

    if (
      (
        count_oscillators %
        0x02
      ) ==
      0x00
    ) {
      count_oscillators = (
        count_oscillators +
        0x01
      );
    }

    for (
      unsigned char index_oscillator = (
        0x00
      );
      (
        index_oscillator <
        count_oscillators
      );
      ++index_oscillator
    ) {
      enum cer0_signal signal;

      if (
        track_lane->type ==
        z_track_lane_type_bass
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
                z_track_parameters->length_signals -
                0x02
              ]
            )
            : (
              z_track_parameters->signals[
                z_track_parameters->length_signals -
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
            z_track_parameters->length_signals
          ]
        );
      }

      cer0_synthesizer_oscillator_add(
        &track_lane->synthesizer,
        signal
      );

      if (
        track_lane->synthesizer.length_oscillators >
        0x01
      ) {
        track_lane->synthesizer.oscillators[
          index_oscillator
        ].pan = (
          (
            index_oscillator ==
            0x00
          )
          ? 0x00
          : (
            (
              (
                index_oscillator %
                0x02
              ) ==
              0x00
            )
            ? -1.0f
            : 1.0f
          )
        );
      }

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
      z_track_parameters->length_notes_multiplier
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

    unsigned char speed;

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

      switch (
        track_lane->type
      ) {
        case z_track_lane_type_bass: {
          speed = (
            0x00
          );

          break;
        }
        case z_track_lane_type_rhythm_chords:
        case z_track_lane_type_rhythm_notes: {
          speed = (
            (
              index_lane *
              (
                index_note +
                index_lane +
                0x01
              )
            ) %
            0x03
          );
          break;
        }
        default: {
          speed = (
            (
              0x02 -
              (
                (
                  index_lane +
                  index_note +
                  track->rand_result.bytes[
                    (
                      index_lane +
                      index_note
                    ) %
                    0x10
                  ]
                ) %
                0x03
              )
            ) +
            0x02
          );

          break;
        }
      }

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
        z_track_parameters->attack_sustain_decay_release_parameters_minimum.attack *
        (
          (
            (
              track_lane->type ==
              z_track_lane_type_rhythm_chords
            ) ||
            (
              track_lane->type ==
              z_track_lane_type_rhythm_notes
            )
          )
          ? 0.5f
          : 0x01
        )
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
        case 0x00: {
          length_note = (
            (float)
            (
              (
                track->rand_result.bytes[
                  0x03
                ] %
                0x02
              ) +
              0x01
            ) *
            whole_beat
          );

          break;
        }
        case 0x01: {
          length_note = (
            (float)
            (
              (
                track->rand_result.bytes[
                  0x03
                ] %
                0x05
              ) +
              0x01
            ) *
            half_beat
          );

          break;
        }
        case 0x02: {
          length_note = (
            (float)
            (
              (
                track->rand_result.bytes[
                  0x09
                ] %
                0x05
              ) +
              0x01
            ) *
            quarter_beat
          );

          break;
        }
        case 0x03: {
          length_note = (
            (float)
            (
              (
                track->rand_result.bytes[
                  0x03
                ] %
                0x05
              ) +
              0x01
            ) *
            eigth_beat
          );

          break;
        }
        case 0x04:
        default: {
          length_note = (
            (float)
            (
              (
                track->rand_result.bytes[
                  0x03
                ] %
                0x05
              ) +
              0x01
            ) *
            sixtenth_beat
          );

          break;
        }
      }

      if (
        index_note >
        0x00
      ) {
        note->time = (
          notes[
            index_note -
            0x01
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
              track->rand_result.bytes[
                0x02
              ] +
              0x01
            )
          ) %
          track->length_scale
        ]
      );

      float value_note;

      if (
        (
          (
            track_lane->type ==
            z_track_lane_type_notes
          ) ||
          (
            track_lane->type ==
            z_track_lane_type_chords
          )
        ) &&
        (
          index_note >=
          0x20
        )
      ) {
        value_note = (
          notes[
            (
              index_note %
              0x08
            ) %
            track_lane->length_notes
          ].value
        );
      } else {
        unsigned int octave_range_minimum;
        unsigned int octave_range;

        if (
          track_lane->type ==
          z_track_lane_type_bass
        ) {
          octave_range_minimum = (
            range_octave_lower_minimum
          );

          octave_range = (
            range_octave_lower
          );
        } else if (
          (
            track_lane->type ==
            z_track_lane_type_rhythm_chords
          ) ||
          (
            track_lane->type ==
            z_track_lane_type_rhythm_notes
          )
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
                  0x04
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
          position_note_table %
          track->length_note_table
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
      0x0258
    );

    track_lane->index_note = (
      0x00
    );

    cer0_synthesizer_frequency_play(
      &track_lane->synthesizer,
      track->note_table[
        notes[
          0x00
        ].value
      ]
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
    unsigned int index_lane = (
      0x00
    );
    (
      index_lane <
      track->length_lanes
    );
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

  for (
    unsigned char index_effect = (
      0x00
    );
    (
      index_effect <
      track->length_effects
    );
    ++index_effect
  ) {
    cer0_effect_destroy(
      &track->effects[
        index_effect
      ]
    );
  }

  clic3_memory_free_raw(
    track->effects
  );

  clic3_memory_free_raw(
    track->lanes
  );
}
