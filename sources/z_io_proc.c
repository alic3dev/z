#include <z_io_proc.h>

#include <z_display.h>
#include <z_io_proc_data.h>
#include <z_queue.h>
#include <z_settings.h>
#include <z_track.h>

#include <cer0_synthesizer.h>
#include <math_c.h>

#include <pthread.h>
#include <stdio.h>

#include <CoreAudio/CoreAudio.h>

OSStatus z_io_proc(
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

    z_queue_destroy(
      &z_io_proc_data->queue
    );

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
      z_io_proc_data->audio_output
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
    
    for (
      unsigned long int index_buffer_out = 0;
      index_buffer_out < size_buffer_out;
      ++index_buffer_out
    ) {
      unsigned long int channel = index_buffer_out % count_channel_out;

      if (channel == 0) {
        z_io_proc_data->frame = (
          z_io_proc_data->frame + 1
        );
        
        float value = 0.0f;

        for (
          unsigned char index_lane = 0;
          index_lane < z_queue->track_current->length_lanes;
          ++index_lane
        ) {
          if (
            (
              (z_io_proc_data->frame + 1) %
              (
                (unsigned long long int) (
                  z_queue->track_current->lanes[
                    index_lane
                  ].notes[
                    z_queue->track_current->lanes[
                      index_lane
                    ].index_note
                  ].time *
                  16.0f
                )
              )
            ) == 0
          ) {
            z_queue->track_current->lanes[
              index_lane
            ].index_note = (
              z_queue->track_current->lanes[
                index_lane
              ].index_note + 1
            ) % z_queue->track_current->lanes[
              index_lane
            ].length_notes;

            cer0_synthesizer_frequency_set(
              &z_queue->track_current->lanes[
                index_lane
              ].synthesizer,
              z_queue->track_current->lanes[
                index_lane
              ].notes[
                z_queue->track_current->lanes[
                  index_lane
                ].index_note
              ].value
            );
          }

          value += cer0_synthesizer_poll(
            &z_queue->track_current->lanes[
              index_lane
            ].synthesizer
          ) * (
            index_lane == (
              z_queue->track_current->length_lanes -
              2
            )
            ? 0.119f
            : index_lane == (
              z_queue->track_current->length_lanes -
              1
            )
            ? 0.191f
            : 1.0f
          );
        }

        buffer_out[
          index_buffer_out
        ] = (
          math_c_floating_point_minimum(
            math_c_floating_point_maximum((
                value / (
                  (float) z_queue->track_current->length_lanes
                )
              ) * z_io_proc_data->settings.volume,
              -1.0f
            ),
            1.0f
          )
        );
      } else {
        buffer_out[
          index_buffer_out
        ] = (
          buffer_out[
            index_buffer_out -
            channel
          ]
        );
      }

      z_queue->track_current->progress = (
        (float) z_io_proc_data->frame / (
          ((float) z_queue->track_current->length) *
          100.0f
        )
      );

      if (
        channel == 0 &&
        z_io_proc_data->frame >= (
          z_queue->track_current->length *
          100
        )
      ) {
        z_io_proc_data->frame = 0;

        z_track_destroy(
          z_queue->track_current
        );

        free(z_queue->track_current);

        z_queue->track_current = (
          z_queue->track_upcoming
        );

        z_queue->track_upcoming = malloc(
          sizeof(struct z_track)
        );

        z_track_generate(
          z_queue->track_upcoming,
          z_queue->audio_output->sample_rate
        );

        z_display_render(
          z_queue
        );
      }
    }
  }

  z_display_render(
    z_queue
  );

  pthread_mutex_unlock(
    &z_io_proc_data->mutex_playing
  );

  return 0;
}
