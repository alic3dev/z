#include <z_track.h>

#include <z_queue.h>
#include <z_track_note.h>
#include <z_track_lane.h>

#include <cer0_octave_range.h>
#include <cer0_frequency_root.h>
#include <cer0_note_table.h>
#include <cer0_scale.h>
#include <cer0_synthesizer.h>

#include <rand_clean.h>
#include <rand_functions.h>
#include <rand_result.h>
#include <rand_source.h>

#include <stdlib.h>

void z_track_generate(
  struct z_track* track,
  float sample_rate
) {
  unsigned char length_scales = 3;

  const unsigned char* z_scales[
    5
  ] = {
    cer0_scale_notes_minor_pentatonic,
    cer0_scale_notes_harmonic_minor,
    cer0_scale_notes_melodic_minor_descending,
    cer0_scale_notes_octatonic_minor,
    cer0_scale_notes_neapolitan_minor
  };

  unsigned char z_lengths_scales[
    5
  ] = {
    cer0_scale_length_minor_pentatonic,
    cer0_scale_length_harmonic_minor,
    cer0_scale_length_melodic_minor_descending,
    cer0_scale_length_octatonic_minor,
    cer0_scale_length_neapolitan_minor
  };

  track->rand_parameters.length = 10;
  track->rand_parameters.mode = rand_mode_bytes;
  track->rand_parameters.error = 0;
  track->rand_parameters.help = 0;
  track->rand_parameters.type_source = rand_source_type_divisive;

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

  track->seed = (
    (struct rand_source_divisive_data*)
    track->rand_source.data
  )->seed;

  unsigned char size_float = (
    sizeof(float)
  );

  for (
    unsigned char index_byte = 0;
    index_byte < size_float &&
    index_byte < 4;
    ++index_byte
  ) {
    track->buffer_seed[
      index_byte
    ] = (
      (unsigned char*)
      &track->seed
    )[
      index_byte
    ];
  }

  for (
    unsigned char index_byte = size_float;
    index_byte < 4;
    ++index_byte
  ) {
    track->buffer_seed[
      index_byte
    ] = 0x0;
  }

  rand_get(
    &track->rand_source,
    &track->rand_result,
    &track->rand_parameters
  );

  unsigned char length_track_name = (
    track->rand_result.bytes[
      0
    ] %
    33 +
    10
  );

  track->name = malloc(
    sizeof(char) *
    length_track_name
  );

  unsigned int octave_minimum = 0;
  unsigned int octave_maximum = 5;

  track->note_table = cer0_note_table_create(
    octave_minimum,
    octave_maximum,
    cer0_frequency_root_standard
  );

  track->range_octave = cer0_octave_range_get(
    octave_minimum,
    octave_maximum
  );

  track->length_note_table = cer0_note_table_length(
    octave_minimum,
    octave_maximum
  );

  track->progress = 0.0f;

  unsigned char index_scale = (
    track->rand_result.bytes[
      3
    ] %
    length_scales
  );

  track->scale = (
    z_scales[
      index_scale
    ]
  );

  track->length_scale = (
    z_lengths_scales[
      index_scale
    ]
  );

  track->key = (
    track->rand_result.bytes[
      4
    ] %
    cer0_default_steps_notes
  );

  for (
    unsigned char index_track_name = 0;
    index_track_name < length_track_name - 1;
    ++index_track_name
  ) {
    rand_get(
      &track->rand_source,
      &track->rand_result,
      &track->rand_parameters
    );

    switch (
      track->rand_result.bytes[0] %
      10
    ) {
      case 0:
      case 1:
      case 2:
        track->name[
          index_track_name
        ] = (
          'a' + (
            track->rand_result.bytes[1] %
            26
          )
        );
        break;
      case 3:
      case 4:
      case 5:
        track->name[
          index_track_name
        ] = (
          'A' + (
            track->rand_result.bytes[1] %
            26
          )
        );
        break;
      case 6:
      case 7:
      case 8:
        track->name[
          index_track_name
        ] = (
          '0' + (
            track->rand_result.bytes[1] %
            10
          )
        );
        break;
      case 9:
        track->name[
          index_track_name
        ] = ' ';
        break;
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
      track->rand_result.bytes[0] *
      track->rand_result.bytes[1]
    ))
  );

  track->length = (float) (
    (float) (
      track->rand_result.bytes[2] +
      track->rand_result.bytes[3]
    ) * 32.0f * 4.0f
  );

  track->length_lanes = (
    track->rand_result.bytes[4] %
    4 + 2
  );

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
      1
    );

    for (
      unsigned char index_oscillator = 0;
      index_oscillator < count_oscillators;
      ++index_oscillator
    ) {
      cer0_synthesizer_oscillator_add(
        &track_lane->synthesizer, (
          index_lane >= (
            track->length_lanes -
            2
          )
          ? index_lane == (
            track->length_lanes -
            2
          )
          ? square
          : square
          : square
        )
      );
    }

    track_lane->length_notes = (
      track->rand_result.bytes[
        4
      ] *
      32
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
      track_lane->length_notes
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
              track->rand_result.bytes[0] *
              track->rand_result.bytes[1]
            ) %
            1000
          ) /
          speed *
          (float) track->length /
          (float) track_lane->length_notes
        );
      } else {
        note->time = (
          (float) (
            (
              track->rand_result.bytes[0] *
              track->rand_result.bytes[1]
            ) %
            1000
          ) /
          speed *
          (float) track->length /
          (float) track_lane->length_notes
        );
      }

      if (
        (index_lane + 1) % 3 == 0
      ) {
        if (
          index_note >= 32
        ) {
          note->value = notes[
            index_note % (index_lane * 4)
          ].value;
        } else {
          note->value = track->note_table[
            (
              track->scale[
                (
                  track->rand_result.bytes[2] *
                  track->rand_result.bytes[3]
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
                track->rand_result.bytes[2] *
                track->rand_result.bytes[3]
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
