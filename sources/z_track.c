#include <z_track.h>

#include <z_queue.h>
#include <z_track_note.h>
#include <z_track_lane.h>
#include <z_words.h>

#include <cer0_octave_range.h>
#include <cer0_note_table.h>
#include <cer0_synthesizer.h>

#include <rand_clean.h>
#include <rand_functions.h>
#include <rand_result.h>
#include <rand_source.h>

#include <stdlib.h>

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

  unsigned char status_rand_get = rand_get(
    &track->rand_source,
    &track->rand_result,
    &track->rand_parameters
  );

  unsigned char size_float = (
    sizeof(float)
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

  track->name = malloc(
    sizeof(char) *
    length_track_name
  );

  track->note_table = cer0_note_table_create(
    z_track_parameters->octave_minimum,
    z_track_parameters->octave_maximum,
    z_track_parameters->frequency_root
  );

  track->range_octave = cer0_octave_range_get(
    z_track_parameters->octave_minimum,
    z_track_parameters->octave_maximum
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

  track->speed = (
    (float) (
    (
      (
        track->rand_result.bytes[0] +
        1
      ) * (
        track->rand_result.bytes[1] +
        1
      )
    ))
  );

  track->length = (float) (
    (float) (
      track->rand_result.bytes[2] +
      track->rand_result.bytes[3]
    ) *
    32.0f *
    z_track_parameters->track_length_multiplier
  );

  track->length_lanes = (
    track->rand_result.bytes[4] % (
      z_track_parameters->track_length_lanes_maximum -
      z_track_parameters->track_length_lanes_minimum
    ) +
    z_track_parameters->track_length_lanes_minimum
  ) * 2;

  if (
    track->length_lanes % 2 != 0
  ) {
    track->length_lanes = (
      track->length_lanes +
      1
    );
  }

  track->lanes = malloc(
    sizeof(
      struct z_track_lane
    ) *
    track->length_lanes
  );

  for (
    unsigned char index_lane = 0;
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
      4 +
      1
    );

    for (
      unsigned char index_oscillator = 0;
      index_oscillator < count_oscillators;
      ++index_oscillator
    ) {
      cer0_synthesizer_oscillator_add(
        &track_lane->synthesizer,
        z_track_parameters->signals[
          track->rand_result.bytes[
            1
          ] %
          z_track_parameters_length_signals_default
        ]
      );

      track_lane->synthesizer.oscillators[
        index_oscillator
      ].amplitude = (
        (float) (
          track->rand_result.bytes[
            2
          ]
        ) /
        255.0f * (
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
      64
    );

    track_lane->notes = malloc(
      sizeof(
        struct z_track_note
      ) *
      track_lane->length_notes
    );

    rand_get(
      &track->rand_source,
      &track->rand_result,
      &track->rand_parameters
    );

    float speed = (
      track->length /
      track_lane->length_notes *
      z_track_parameters->track_speed_multiplier
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

      if (
        index_note > 0
      ) {
        note->time = (
          notes[
            index_note -
            1
          ].time +
          (float) (
            (
              (
                track->rand_result.bytes[0] +
                1
              ) * (
                track->rand_result.bytes[1] +
                1
              )
            ) %
            1000 +
            1
          ) /
          speed *
          (float) track->length /
          (float) track_lane->length_notes
        );
      } else {
        note->time = (
          (float) (
            (
              (
                track->rand_result.bytes[0] +
                1
              ) * (
                track->rand_result.bytes[1] +
                1
              )
            ) %
            1000 +
            1
          ) /
          speed *
          (float) track->length /
          (float) track_lane->length_notes
        );
      }

      if (
        index_lane < 2
      ) {
        if (
          index_note >= 32
        ) {
          note->value = notes[
            (index_note % 8) %
            track_lane->length_notes
          ].value;
        } else {
          note->value = track->note_table[
            (
              track->scale[
                (
                  (
                    track->rand_result.bytes[2] +
                    1
                  ) * (
                    track->rand_result.bytes[3] +
                    1
                  )
                ) %
                track->length_scale
              ] + (
                cer0_default_steps_notes * (
                  track->rand_result.bytes[4] %
                  track->range_octave
                )
              ) +
              track->key
            ) %
            track->length_note_table
          ];
        }
      } else {
        note->value = track->note_table[
          (
            track->scale[
              (
                (
                  track->rand_result.bytes[2] +
                  1
                ) * (
                  track->rand_result.bytes[3] +
                  1
                )
              ) %
              track->length_scale
            ] + (
              cer0_default_steps_notes * (
                track->rand_result.bytes[4] %
                track->range_octave
              )
            ) +
            track->key
          ) %
          track->length_note_table
        ];
      }
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
  free(
    track->name
  );

  free(
    track->note_table
  );

  rand_clean(
    &track->rand_result,
    &track->rand_source
  );

  for (
    unsigned char index_lane = 0;
    index_lane < track->length_lanes;
    ++index_lane
  ) {
    cer0_synthesizer_destroy(
      &track->lanes[
        index_lane
      ].synthesizer
    );

    free(
      track->lanes[
        index_lane
      ].notes
    );
  }

  free(
    track->lanes
  );
}
