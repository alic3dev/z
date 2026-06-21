#include <z_io_proc.h>

#include <z_event.h>
#include <z_io_proc_data.h>
#include <z_queue.h>
#include <z_settings.h>
#include <z_track.h>

#include <clic3_bytes.h>
#include <clic3_memory.h>

#include <cer0_synthesizer.h>

#include <math_c_absolute.h>
#include <math_c_bound.h>
#include <math_c_maximum.h>
#include <math_c_minimum.h>
#include <math_c_modulus.h>
#include <math_c_pi.h>
#include <math_c_sine.h>

#include <pthread.h>

#if !target_os_ios
#include <CoreAudio/CoreAudio.h>

void* z_io_proc_initializer(
  void* data
) {
  struct z_io_proc_data* z_io_proc_data = (
    data
  );

  z_queue_initialize(
    &z_io_proc_data->queue,
    &z_io_proc_data->track_parameters,
    z_io_proc_data->rate_sample
  );

  z_io_proc_data->z_close_exit_data->initializing = (
    0x00
  );

  z_io_proc_data->initialized = (
    0x01
  );

  return (
    0x00
  );
}

int z_io_proc(
  AudioObjectID id_audio_object,
  const AudioTimeStamp* timestamp_audio,
  const AudioBufferList* buffer_list_audio_in,
  const AudioTimeStamp* timestamp_audio_in,
  AudioBufferList* buffer_list_audio_out,
  const AudioTimeStamp* timestamp_audio_out,
  void* data
) {
  struct z_io_proc_data* z_io_proc_data = (
    data
  );

  if (
    z_io_proc_data->exiting ==
    0x02
  ) {
    return (
      0x00
    );
  }

  if (
    z_io_proc_data->exiting ==
    0x01
  ) {
    pthread_mutex_lock(
      &z_io_proc_data->mutex_playing
    );

    z_io_proc_data->exiting = (
      0x02
    );

    pthread_mutex_unlock(
      &z_io_proc_data->mutex_exited
    );

    pthread_mutex_unlock(
      &z_io_proc_data->mutex_playing
    );

    return (
      0x00
    );
  }

  if (
    z_io_proc_data->initialized ==
    0x00
  ) {
    z_io_proc_data->initialized = (
      0x02
    );

    pthread_t z_pthread_initializer;

    pthread_create(
      &z_pthread_initializer,
      0x00,
      z_io_proc_initializer,
      z_io_proc_data
    );

    return (
      0x00
    );
  } else if (
    z_io_proc_data->initialized ==
    0x02
  ) {
    return (
      0x00
    );
  }

  pthread_mutex_lock(
    &z_io_proc_data->mutex_playing
  );

  if (
    z_io_proc_data->exiting !=
    0x00
  ) {
    return (
      0x00
    );
  }

  struct z_queue* z_queue = &(
    z_io_proc_data->queue
  );

  for (
    unsigned long int index_buffer = (
      0x00
    );
    (
      index_buffer <
      buffer_list_audio_out->mNumberBuffers
    );
    ++index_buffer
  ) {
    AudioBuffer audio_buffer_current = (
      buffer_list_audio_out->mBuffers[
        index_buffer
      ]
    );

    float* buffer_out = (
      audio_buffer_current.mData
    );

    unsigned long int size_buffer_out = (
      audio_buffer_current.mDataByteSize /
      sizeof(
        float
      )
    );

    unsigned long int count_channel_out = (
      audio_buffer_current.mNumberChannels
    );

    float pan = (
      0.5f
    );

    float buffer_out_channel_zero_value = (
      0x00
    );

    for (
      unsigned long int index_buffer_out = (
        0x00
      );
      (
        index_buffer_out <
        size_buffer_out
      );
      ++index_buffer_out
    ) {
      unsigned long int channel = (
        index_buffer_out %
        count_channel_out
      );

      if (
        channel ==
        0x00
      ) {
        z_io_proc_frame_get(
          z_io_proc_data,
          z_queue,
          buffer_out,
          index_buffer_out,
          channel
        );

        buffer_out_channel_zero_value = (
          buffer_out[
            index_buffer_out
          ]
        );

        buffer_out_channel_zero_value = (
          buffer_out_channel_zero_value *
          (
            math_c_absolute_float(
              math_c_sine(
                (float) z_io_proc_data->frame /
                (
                  (
                    0x3c /
                    z_queue->track_current->bpm *
                    0x03e8
                  ) *
                  (
                    *z_io_proc_data->rate_sample /
                    0x0258
                  ) *
                  0x20
                )  *
                math_c_pi,
                math_c_pi
              )
            ) *
            0.75f +
            0.25f
          )
        );

        buffer_out[
          index_buffer_out
        ] = (
          buffer_out_channel_zero_value *
          (
            0x01 -
            pan
          )
        );
      } else {
        buffer_out[
          index_buffer_out
        ] = (
          buffer_out_channel_zero_value *
          pan
        );
      }
    }
  }

  z_event_trigger(
    &z_queue->event_store,
    z_event_type_track_update,
    z_queue
  );

  pthread_mutex_unlock(
    &z_io_proc_data->mutex_playing
  );

  return (
    0x00
  );
}
#endif

float z_io_proc_frame_value_get(
  struct z_track* z_track,
  unsigned long long int index_frame,
  float rate_sample
) {
  float value = (
    0x00
  );

  float value_last = (
    0x00
  );

  for (
    unsigned int index_lane = (
      0x00
    );
    (
      index_lane <
      z_track->length_lanes
    );
    ++index_lane
  ) {
    struct z_track_lane* track_lane = &(
      z_track->lanes[
        index_lane
      ]
    );

    struct z_track_note* notes = (
      track_lane->notes
    );

    struct z_track_note* note = &(
      notes[
        track_lane->index_note
      ]
    );

    unsigned long int note_life_end = (
      (
        rate_sample /
        0x0258
      ) *
      note->time
    );

    unsigned long int note_life = (
      (
        index_frame +
        0x01
      ) %
      note_life_end
    );

    if (
      note_life ==
      0x00
    ) {
      track_lane->index_note = (
        (
          track_lane->index_note +
          0x01
        ) %
        track_lane->length_notes
      );

      note = &(
        notes[
          track_lane->index_note
        ]
      );

      clic3_bytes_copy(
        &track_lane->synthesizer.attack_sustain_decay_release_parameters,
        &note->attack_sustain_decay_release_parameters,
        sizeof(
          struct cer0_attack_sustain_decay_release_parameters
        )
      );

      track_lane->synthesizer.length_attack_sustain_decay_release = (
        (
          rate_sample /
          0x0258
        ) *
        note->time -
        index_frame
      );

      switch (
        track_lane->type
      ) {
        case z_track_lane_type_chords:
        case z_track_lane_type_rhythm_chords: {
          track_lane->synthesizer.index_attack_sustain_decay_release = (
            0x00
          );

          for (
            unsigned char index_oscillator = (
              0x00
            );
            (
              index_oscillator <
              track_lane->synthesizer.length_oscillators
            );
            ++index_oscillator
          ) {
            cer0_synthesizer_oscillator_frequency_set(
              &track_lane->synthesizer,
              index_oscillator,
              z_track->note_table[
                (
                  note->value +
                  (
                    (
                      index_oscillator +
                      (
                        track_lane->index_note +
                        0x01
                      )
                    ) %
                    0x04 +
                    0x01
                  ) *
                  0x02
                ) %
                z_track->length_note_table
              ]
            );
          }

          break;
        }
        default: {
          float frequency = (
            z_track->note_table[
              note->value
            ]
          );
        
          for (
            unsigned int i = 0;
            i < track_lane->synthesizer.length_oscillators;
            ++i
          ) {
            track_lane->synthesizer.oscillators[
              i
            ].offset_frequency = (
              frequency /
              0x64 /
              0x04 *
              i
            );  
          }
        
          cer0_synthesizer_frequency_play(
            &track_lane->synthesizer,
            frequency
          );

          break;
        }
      }
    }

    if (
      index_lane ==
      (
        z_track->length_lanes -
        0x01
      )
    ) {
      value_last = (
        value_last +
        cer0_synthesizer_poll(
          &track_lane->synthesizer
        ) *
        note->amplitude
      );
    } else {
      value = (
        value +
        cer0_synthesizer_poll(
          &track_lane->synthesizer
        ) *
        note->amplitude
      );
    }
  }

  value = (
    value /
    (float)
    z_track->length_lanes
  );

  value_last = (
    value_last /
    (float)
    z_track->length_lanes
  );

  for (
    unsigned int index_effect = (
      0x00
    );
    (
      index_effect <
      z_track->length_effects
    );
    ++index_effect
  ) {
    value = (
      cer0_effect_poll(
        &z_track->effects[
          index_effect
        ],
        0x00,
        value
      )
    );
  }

  return (
    value +
    value_last
  );
}

float z_io_proc_frame_volume_apply(
  float frame_value,
  float volume
) {
  return (
    frame_value *
    volume
  );
}

void z_io_proc_frame_get(
  struct z_io_proc_data* z_io_proc_data,
  struct z_queue* z_queue,
  float* buffer_out,
  unsigned long int index_buffer_out,
  unsigned long int channel
) {
  if (
    channel ==
    0x00
  ) {
    z_io_proc_data->frame = (
      z_io_proc_data->frame +
      0x01
    );
  }

  buffer_out[
    index_buffer_out
  ] = (
    z_io_proc_frame_volume_apply(
      z_io_proc_frame_value_get(
        z_queue->track_current,
        z_io_proc_data->frame,
        *z_queue->rate_sample
      ),
      z_io_proc_data->settings.volume
    )
  );

  if (
    channel ==
    0x00
  ) {
    z_queue->track_current->progress = (
      (float)
      z_io_proc_data->frame /
      (float)
      (
        z_queue->track_current->length *
        0x64
      )
    );

    if (
      z_io_proc_data->frame >=
      (
        z_queue->track_current->length *
        0x64
      )
    ) {
      z_io_proc_data->frame = (
        0x00
      );

      z_queue_track_next(
        z_queue
      );
    }
  }
}
