#include <track.h>

#include <display.h>
#include <value.h>
#include <queue.h>

#include <cer0_octave_range.h>
#include <cer0_frequency_root.h>
#include <cer0_note_table.h>
#include <cer0_scale.h>
#include <cer0_synthesizer.h>

#include <stdlib.h>

const unsigned char length_scales = 4;

const unsigned char* scales[length_scales] = {
  cer0_scale_notes_harmonic_minor,
  cer0_scale_notes_minor_pentatonic,
  cer0_scale_notes_minor_pentatonic,
  cer0_scale_notes_minor_pentatonic
};

const unsigned char lengths_scales[length_scales] = {
  cer0_scale_length_harmonic_minor,
  cer0_scale_length_minor_pentatonic,
  cer0_scale_length_minor_pentatonic,
  cer0_scale_length_minor_pentatonic
};

float sample_rate = 44100.0f;

void track_generate(
  struct track* track
) {
  value_seed();

  track->seed = value_seed_value;
  value_buffer_seed_set(track->buffer_seed);

  unsigned char length_track_name = (int) value_get() % 33 + 10;

  track->name = malloc(
    sizeof(char) *
    length_track_name
  );

  unsigned int octave_minimum = (int) value_get() % 2 + 2;
  unsigned int octave_maximum = (int) value_get() % 2 + 1 + octave_minimum;

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

  unsigned char index_scale = (int) value_get() % length_scales;
  track->scale = (unsigned char*) scales[index_scale];
  track->length_scale = lengths_scales[index_scale];
  track->key = (int) value_get() % cer0_default_steps_notes;

  for (
    unsigned char index_track_name = 0;
    index_track_name < length_track_name - 1;
    ++index_track_name
  ) {
    switch ((int) value_get() % 10) {
      case 0:
      case 1:
      case 2:
        track->name[
          index_track_name
        ] = 'a' + ((int) value_get() % 26);
        break;
      case 3:
      case 4:
      case 5:
        track->name[
          index_track_name
        ] = 'A' + ((int) value_get() % 26);
        break;
      case 6:
      case 7:
      case 8:
        track->name[
          index_track_name
        ] = '0' + ((int) value_get() % 10);
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

  track->speed = (float)((int) value_get() % 1000) / 1000.0f;
  track->length = (float)((int) value_get() % 1000 * 100 + 10000);

  track->length_lanes = (int) value_get() % 10 + 4;
  track->lanes = malloc(
    sizeof(struct track_lane) *
    track->length_lanes
  );

  for (
    unsigned char index_lane = 0;
    index_lane < track->length_lanes;
    ++index_lane
  ) {
    cer0_synthesizer_initialize(
      &track->lanes[
        index_lane
      ].synthesizer,
      sample_rate
    );

    unsigned char count_oscillators = ((int) value_get() % 4) + 2;

    for (
      unsigned char index_oscillator = 0;
      index_oscillator < count_oscillators;
      ++index_oscillator
    ) {
      cer0_synthesizer_oscillator_add(
        &track->lanes[
          index_lane
        ].synthesizer,
        ((int) value_get() % (cer0_length_signals - 2)) + 1
      );
    }

    track->lanes[
      index_lane
    ].length_notes = (int) value_get() % 400 + 100;

    track->lanes[
      index_lane
    ].notes = malloc(
      sizeof(struct track_note) *
      track->lanes[
        index_lane
      ].length_notes
    );

    for (
      unsigned long int index_note = 0;
      index_note < track->lanes[
        index_lane
      ].length_notes;
      ++index_note
    ) {
      track->lanes[
        index_lane
      ].notes[
        index_note
      ].time = (
        (float)((int) value_get() % 1000) /
        1000.0f *
        track->length
      );

      track->lanes[
        index_lane
      ].notes[
        index_note
      ].value = track->note_table[(
        track->scale[
           (int) value_get() % track->length_scale
        ] + (
          cer0_default_steps_notes *
          ((int) value_get() % track->range_octave)
        ) + track->key
      ) % track->length_note_table];
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

void track_destroy(
  struct track* track
) {
  free(track->name);
  free(track->note_table);

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

  free(track->lanes);

  free(track);
}
