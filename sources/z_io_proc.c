#include <z_io_proc.h>

#include <z_event.h>
#include <z_io_proc_data.h>
#include <z_queue.h>
#include <z_settings.h>
#include <z_track.h>

#include <clic3_memory.h>

#include <cer0_synthesizer.h>
#include <math_c_maximum.h>
#include <math_c_minimum.h>

#include <pthread.h>
#include <stdio.h>

#if !target_os_ios
#include <CoreAudio/CoreAudio.h>

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
    z_io_proc_data->exiting == 2
  ) {
    return 0;
  }

  if (
    z_io_proc_data->exiting == 1
  ) {
    pthread_mutex_lock(
      &z_io_proc_data->mutex_playing
    );

    z_io_proc_data->exiting = 2;

    pthread_mutex_unlock(
      &z_io_proc_data->mutex_exited
    );

    pthread_mutex_unlock(
      &z_io_proc_data->mutex_playing
    );

    return 0;
  }

  if (
    z_io_proc_data->initialized == 0
  ) {
    z_queue_initialize(
      &z_io_proc_data->queue,
      z_io_proc_data->track_parameters,
      z_io_proc_data->rate_sample
    );

    z_io_proc_data->z_close_exit_data->initializing = (
      0
    );

    z_io_proc_data->initialized = 1;
  }

  pthread_mutex_lock(
    &z_io_proc_data->mutex_playing
  );

  if (
    z_io_proc_data->exiting != 0
  ) {
    return 0;
  }

  struct z_queue* z_queue = &(
    z_io_proc_data->queue
  );

  for (
    unsigned long int index_buffer = 0;
    index_buffer < buffer_list_audio_out->mNumberBuffers;
    ++index_buffer
  ) {
    AudioBuffer audio_buffer_current = (
      buffer_list_audio_out->mBuffers[
        index_buffer
      ]
    );

    float* buffer_out = audio_buffer_current.mData;
    unsigned long int size_buffer_out = audio_buffer_current.mDataByteSize / sizeof(float);
    unsigned long int count_channel_out = audio_buffer_current.mNumberChannels;

    float pan = 0.5f;
    float buffer_out_channel_zero_value = 0.0f;
    
    for (
      unsigned long int index_buffer_out = 0;
      index_buffer_out < size_buffer_out;
      ++index_buffer_out
    ) {
      unsigned long int channel = index_buffer_out % count_channel_out;

      if (
        channel == 0
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

        pan = (
          0.5f + (
            buffer_out_channel_zero_value +
            1.0f
          ) /
          4.0f
        );

        buffer_out[
          index_buffer_out
        ] = (
          buffer_out_channel_zero_value *
          math_c_minimum_float(
            (
              1.0f -
              pan
            ) *
            2.0f,
            1.0f
          )
        );
      } else {
        buffer_out[
          index_buffer_out
        ] = (
          buffer_out_channel_zero_value *
          math_c_minimum_float(
            pan *
            2.0f,
            1.0f
          )
        );
      }
    }
  }

  z_event_trigger(
    z_event_type_track_update,
    z_queue
  );

  pthread_mutex_unlock(
    &z_io_proc_data->mutex_playing
  );

  return 0;
}
#endif

void z_io_proc_frame_get(
  struct z_io_proc_data* z_io_proc_data,
  struct z_queue* z_queue,
  float* buffer_out,
  unsigned long int index_buffer_out,
  unsigned long int channel
) {
  if (
    channel == 0
  ) {
    z_io_proc_data->frame = (
      z_io_proc_data->frame + 1
    );
  }
  
  float value = 0.0f;

  for (
    unsigned char index_lane = 0;
    index_lane < z_queue->track_current->length_lanes;
    ++index_lane
  ) {
    struct z_track_lane* track_lane = &(
      z_queue->track_current->lanes[
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

    unsigned int note_life_end = (
      (unsigned long long int) (
        (
          
          *z_io_proc_data->rate_sample /
          600.0f
        ) *
        note->time
      )
    );

    unsigned int note_life = (
      (z_io_proc_data->frame + 1) %
      note_life_end
    );
    
    if (
      note_life == 0
    ) {
      track_lane->index_note = (
        (
          track_lane->index_note + 1
        ) % 
        track_lane->length_notes
      );

      note = &(
        notes[
          track_lane->index_note
        ]
      );

      cer0_synthesizer_frequency_set(
        &track_lane->synthesizer,
        note->value
      );
    }

    value = (
      value + (
        cer0_synthesizer_poll(
          &track_lane->synthesizer
        ) * (
          (note_life > (note_life_end / 2))
          ? (
            1.0f - (
              (float) (
                note_life > note_life_end
                ? note_life_end
                : note_life
              ) /
              (float) (
                note_life_end
              )
            )
          ) *
          note->release +
          (1.0f - note->release)
          : (
            (float) (
              note_life
            ) /
            (float) (
              note_life_end
            )
          ) *
          note->attack +
          (1.0f - note->attack)
        ) *
        note->amplitude
      )
    );
  }

  buffer_out[
    index_buffer_out
  ] = (
    math_c_minimum_float(
      math_c_maximum_float((
          value /
          (float) z_queue->track_current->length_lanes
        ) *
        z_io_proc_data->settings.volume,
        -1.0f
      ),
      1.0f
    )
  );

  if (
    channel == 0
  ) {
    z_queue->track_current->progress = (
      (float) z_io_proc_data->frame / (
        ((float) z_queue->track_current->length) *
        100.0f
      )
    );

    if (
      z_io_proc_data->frame >= (
        z_queue->track_current->length *
        100
      )
    ) {
      z_io_proc_data ->frame = 0;

      z_queue_track_next(
        z_queue
      );
    }
  }
}
