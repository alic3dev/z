#include <z_track.h>

#include <z_queue.h>
#include <z_track_note.h>
#include <z_track_lane.h>
#include <z_words.h>

#include <clic3_base_hexadecimal.h>
#include <clic3_memory.h>

#include <cer0_octave_range.h>
#include <cer0_note_table.h>
#include <cer0_synthesizer.h>

#include <math_c_maximum.h>

#include <rand_clean.h>
#include <rand_functions.h>
#include <rand_result.h>
#include <rand_source.h>

void z_track_generate(
  struct z_track* track,
  struct z_track_parameters* z_track_parameters,
  float sample_rate
) {
  track->rand_parameters.length = 10;
  track->rand_parameters.mode = rand_mode_bytes;
  track->rand_parameters.error = 0;
  track->rand_parameters.help = 0;

  track->rand_parameters.type_source = (
    z_track_parameters->rand_source_type
  );

  track->rand_source_parameters.type_source = (
    track->rand_parameters.type_source
  );

  rand_source_initialize(
    &track->rand_source,
    &track->rand_source_parameters
  );

  struct rand_result rand_result;

  rand_result_initialize(
    &track->rand_result,
    track->rand_parameters.length
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
        0
      ] %
      6
    ) +
    2
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
      1
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

  track->note_table = cer0_note_table_create(
    z_track_parameters->octave_minimum,
    z_track_parameters->octave_maximum,
    z_track_parameters->frequency_root
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

  track->length_note_table = cer0_note_table_length(
    z_track_parameters->octave_minimum,
    z_track_parameters->octave_maximum
  );

  track->progress = 0.0f;

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
    unsigned char index_track_name = 0;
    index_track_name < length_track_name - 1;
    ++index_track_name
  ) {
    track->name[
      index_track_name
    ] = (
      track->rand_result.bytes[
        index_track_name
      ] %
      126 +
      1
    );

    for (
      unsigned char index_track_name_lookup = 0;
      index_track_name_lookup < index_track_name;
    ) {
      if (
        track->name[
          index_track_name
        ] == track->name[
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
            1 
          ) %
          126 +
          1
        );

        index_track_name_lookup = 0;
      } else {
        index_track_name_lookup = (
          index_track_name_lookup +
          1
        );
      }
    }
  }

  track->name[
    length_track_name -
    1
  ] = '\0';

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
        track->rand_result.bytes[0] +
        1
      ) + (
        track->rand_result.bytes[1] +
        1
      )
    ) /
    510.0f *
    track_bpm_range
  );

  float whole_beat = (
    (
      60.0f /
      track->bpm
    ) *
    1000.0f
  );

  float half_beat = (
    whole_beat /
    2.0f
  );

  float quarter_beat = (
    whole_beat /
    4.0f
  );

  float eigth_beat = (
    whole_beat /
    8.0f
  );

  float sixtenth_beat = (
    whole_beat /
    16.0f
  );

  track->length = (
    (float) (
      track->rand_result.bytes[2] +
      track->rand_result.bytes[3]
    ) *
    32.0f *
    z_track_parameters->track_length_multiplier
  );

  track->length_lanes = (
    (
      track->rand_result.bytes[
        4
      ] %
      (
        z_track_parameters->track_length_lanes_maximum -
        z_track_parameters->track_length_lanes_minimum
      ) +
      z_track_parameters->track_length_lanes_minimum
    ) *
    2
  );

  if (
    track->length_lanes % 2 != 0
  ) {
    track->length_lanes = (
      track->length_lanes +
      1
    );
  }

  track->lanes = (
    clic3_memory_allocate_raw(
      sizeof(
        struct z_track_lane
      ) *
      track->length_lanes
    )
  );

  for (
    unsigned int index_lane = 0;
    index_lane < track->length_lanes;
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

    unsigned char count_oscillators = (
      track->rand_result.bytes[
        0
      ] %
      1 +
      1
    );

    for (
      unsigned char index_oscillator = 0;
      index_oscillator < count_oscillators;
      ++index_oscillator
    ) {
      enum cer0_signal signal;

      if (
        index_lane == 0
      ) {
        signal = (
          track->rand_result.bytes[
            1
          ] <= 200
          ? z_track_parameters->signals[
            0
          ]
          : (
            track->rand_result.bytes[
              1
            ] <= 230
          )
          ? z_track_parameters->signals[
            z_track_parameters_length_signals_default -
            2
          ]
          : z_track_parameters->signals[
            z_track_parameters_length_signals_default -
            1
          ]
        );
      } else {
        signal = (
          z_track_parameters->signals[
            track->rand_result.bytes[
              1
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
            2
          ]
        ) /
        255.0f *
        (
          z_track_parameters->oscillator_amplitude_maximum -
          z_track_parameters->oscillator_amplitude_minimum
        ) +
        z_track_parameters->oscillator_amplitude_minimum
      );
    }

    track_lane->length_notes = (
      (
        track->rand_result.bytes[
          4
        ] %
        16 +
        8
      ) *
      256
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
      index_lane
    );

    struct z_track_note* notes = (
      track_lane->notes
    );

    for (
      unsigned long int index_note = 0;
      index_note < track_lane->length_notes;
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
        (float) track->rand_result.bytes[7] /
        255.0f *
        (
          z_track_parameters->note_amplitude_maximum -
          z_track_parameters->note_amplitude_minimum
        ) +
        z_track_parameters->note_amplitude_minimum
      );

      note->attack = (
        (
          (float) track->rand_result.bytes[5] /
          255.0f
        ) *
        (
          z_track_parameters->note_attack_maximum -
          z_track_parameters->note_attack_minimum
        ) +
        z_track_parameters->note_attack_minimum
      );

      note->release = (
        (
          (float) track->rand_result.bytes[6] /
          255.0f
        ) * (
          z_track_parameters->note_release_maximum -
          z_track_parameters->note_release_minimum
        ) +
        z_track_parameters->note_release_minimum
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
              64
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
              16
            ) *
            half_beat
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
                ] % 3
              ) +
              1
            ) *
            sixtenth_beat
          );

          break;
        }
        default:
        case 3: {
          length_note = (
            (float)
            (
              (
                track->rand_result.bytes[
                  3
                ] % 3
              ) *
              2 +
              1
            ) *
            sixtenth_beat
          );

          break;
        }
      }

      if (
        (index_lane + index_note) % 11 == 7
      ) {
        length_note = (
          2.0f *
          length_note
        );
      }

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

      float value_note;

      unsigned int position_scale = (
        (
          track->rand_result.bytes[
            0
          ]
        ) %
        track->length_scale
      );
      
      unsigned int position_note_table = (
        (
          (
          track->scale[
            position_scale
          ] +
          track->key
        )
           +
          (
            (
              cer0_default_steps_notes *
              (
                (
                  (unsigned int) note->time +
                  index_note *
                  (
                    index_lane -
                    track->length_lanes
                  )
                ) /
                track->length_scale
              )
            ) %
            (unsigned long int) ((float) (
              z_track_parameters->octave_maximum -
              z_track_parameters->octave_minimum
            ) * (
              (
                (float) (
                  index_lane +
                  1
                ) / (float) (
                  track->length_lanes
                )
              )
            ))
          )
        )
      );

      value_note = (
        track->note_table[
          position_note_table %
          track->length_note_table
        ]
      );

      note->value = (
        value_note
      );
    }

    cer0_synthesizer_frequency_set(
      &track_lane->synthesizer,
      notes[
        0
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
