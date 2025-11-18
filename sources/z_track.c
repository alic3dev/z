#include <z_track.h>

#include <z_queue.h>

#include <cer0_octave_range.h>
#include <cer0_frequency_root.h>
#include <cer0_note_table.h>
#include <cer0_scale.h>
#include <cer0_synthesizer.h>

#include <rand_functions.h>
#include <rand_result.h>
#include <rand_source.h>

#include <stdlib.h>


void z_track_generate(
  struct z_track* track,
  float sample_rate
) {
  unsigned char length_scales = 1;

  const unsigned char* z_scales[
    1
  ] = {
    cer0_scale_notes_major_pentatonic
  };

  unsigned char z_lengths_scales[
    1
  ] = {
    cer0_scale_length_major_pentatonic
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
    (struct rand_source_divisive_data*) track->rand_source.data
  )->seed;

  for (
    unsigned char index_byte = 0;
    index_byte < sizeof(float);
    ++index_byte
  ) {
    track->buffer_seed[
      index_byte
    ] = (
      (unsigned char*) &track->seed
    )[
      index_byte
    ];
  }

  rand_get(
    &track->rand_source,
    &track->rand_result,
    &track->rand_parameters
  );

  unsigned char length_track_name = (
    track->rand_result.bytes[
      0
    ] % 33 + 10
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
    ] % length_scales
  );
  track->scale = (
    z_scales[
      index_scale
    ]
  );
  track->length_scale = z_lengths_scales[index_scale];
  track->key = (
    track->rand_result.bytes[
      4
    ]
  ) % cer0_default_steps_notes;

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
        ] = 'a' + (track->rand_result.bytes[1] % 26);
        break;
      case 3:
      case 4:
      case 5:
        track->name[
          index_track_name
        ] = 'A' + (track->rand_result.bytes[1] % 26);
        break;
      case 6:
      case 7:
      case 8:
        track->name[
          index_track_name
        ] = '0' + (track->rand_result.bytes[1] % 10);
        break;
      case 9:
        track->name[
          index_track_name
        ] = ' ';
        break;
    }
  }

  track->name[
    length_track_name - 1
  ] = '\0';

  rand_get(
    &track->rand_source,
    &track->rand_result,
    &track->rand_parameters
  );

  track->speed = (float)(
    (
      track->rand_result.bytes[0] *
      track->rand_result.bytes[1]
    ) % 119
  ) / 119.0f;

  track->length = (float)(
    (
      track->rand_result.bytes[2] *
      track->rand_result.bytes[3]
    ) % 11 * 9.1f + 1 + 119 * 911 * 119
  );

  track->length_lanes = (
    track->rand_result.bytes[4]
  ) % 9 + 11;

  track->lanes = malloc(
    sizeof(struct z_track_lane) *
    track->length_lanes
  );

  for (
    unsigned char index_lane = 0;
    index_lane < track->length_lanes;
    ++index_lane
  ) {
    rand_get(
      &track->rand_source,
      &track->rand_result,
      &track->rand_parameters
    );

    cer0_synthesizer_initialize(
      &track->lanes[
        index_lane
      ].synthesizer,
      sample_rate
    );

    unsigned char count_oscillators = (
      track->rand_result.bytes[0] % 11
    ) + 9;

    for (
      unsigned char index_oscillator = 0;
      index_oscillator < count_oscillators;
      ++index_oscillator
    ) {
      cer0_synthesizer_oscillator_add(
        &track->lanes[
          index_lane
        ].synthesizer, (
          index_lane >= (track->length_lanes - 2)
          ? index_lane == (track->length_lanes - 2)
          ? 3
          : 4
          : 2
        )
      );
    }

    track->lanes[
      index_lane
    ].length_notes = (
      track->rand_result.bytes[
        4
      ]
    ) % (1 * (index_lane + 1)) * 5000;

    track->lanes[
      index_lane
    ].notes = malloc(
      sizeof(struct z_track_note) *
      track->lanes[
        index_lane
      ].length_notes
    );

    rand_get(
      &track->rand_source,
      &track->rand_result,
      &track->rand_parameters
    );

    float speed = ((float) ((
      track->rand_result.bytes[5] *
      track->rand_result.bytes[6]
    ) % 9) + 11) * 119.0f / (float) (index_lane + 1);

    for (
      unsigned long int index_note = 0;
      index_note < track->lanes[
        index_lane
      ].length_notes;
      ++index_note
    ) {
      rand_get(
        &track->rand_source,
        &track->rand_result,
        &track->rand_parameters
      );

      if (
        index_note > 0
      ) {
        track->lanes[
          index_lane
        ].notes[
          index_note
        ].time = (
          track->lanes[
            index_lane
          ].notes[
            index_note - 1
          ].time +
          (float)((
              track->rand_result.bytes[0] *
              track->rand_result.bytes[1]
            ) % 1000
          ) /
          speed *
          (float) track->length / (float) track->lanes[
            index_lane
          ].length_notes
        );
      } else {
        track->lanes[
          index_lane
        ].notes[
          index_note
        ].time = (
          (float)((
              track->rand_result.bytes[0] *
              track->rand_result.bytes[1]
            ) % 1000
          ) /
          speed *
          (float) track->length / (float) track->lanes[
            index_lane
          ].length_notes
        );
      }

      track->lanes[
        index_lane
      ].notes[
        index_note
      ].value = track->note_table[
        (
          track->scale[
            (
              track->rand_result.bytes[2] *
              track->rand_result.bytes[3]
            ) % track->length_scale
          ] + (
            cer0_default_steps_notes * (
              track->rand_result.bytes[4] %
              track->range_octave
            )
          ) + track->key
        ) % track->length_note_table
      ];
    }

    cer0_synthesizer_frequency_set(
      &track->lanes[
        index_lane
      ].synthesizer,
      track->lanes[
        index_lane
      ].notes[
        0
      ].value
    );
  }

  track->complete = 0;
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

  rand_result_clean(
    &track->rand_result
  );

  rand_source_clean(
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
