#include <io_proc.h>

#include <display.h>
#include <io_proc_data.h>
#include <queue.h>
#include <track.h>

#include <cer0_synthesizer.h>
#include <math_c.h>

#include <pthread.h>
#include <stdio.h>

#include <CoreAudio/CoreAudio.h>

OSStatus io_proc(
  AudioObjectID id_audio_object,
  const AudioTimeStamp* timestamp_audio,
  const AudioBufferList* buffer_list_audio_in,
  const AudioTimeStamp* timestamp_audio_in,
  AudioBufferList* buffer_list_audio_out,
  const AudioTimeStamp* timestamp_audio_out,
  void* data
) {
  struct io_proc_data* io_proc_data = (struct io_proc_data*) data;

  if (
    io_proc_data->initialized == 0
  ) {
    return 0;
  }

  struct track* track = track_current;

  if (
    track == (void*)0 ||
    track->complete == 1
  ) {
    track = track_upcoming;
  }

  for (
    unsigned long int index_buffer = 0;
    index_buffer < buffer_list_audio_out->mNumberBuffers;
    ++index_buffer
  ) {
    AudioBuffer audio_buffer_current = buffer_list_audio_out->mBuffers[index_buffer];

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
        io_proc_data->frame = (
          io_proc_data->frame + 1
        );
      }

      if (channel == 0) {
        float value = 0.0f;

        for (
          unsigned char index_lane = 0;
          index_lane < track->length_lanes;
          ++index_lane
        ) {
          if (
            ((io_proc_data->frame + 1) % ((unsigned long long int) (track->lanes[
              index_lane
            ].notes[
              track->lanes[
                index_lane
              ].index_note
            ].time * 16.0f))) == 0
          ) {
            track->lanes[
              index_lane
            ].index_note = (
              track->lanes[
                index_lane
              ].index_note + 1
            ) % track->lanes[
              index_lane
            ].length_notes;

            cer0_synthesizer_frequency_set(
              &track->lanes[
                index_lane
              ].synthesizer,
              track->lanes[
                index_lane
              ].notes[
                track->lanes[
                  index_lane
                ].index_note
              ].value
            );
          }

          value += cer0_synthesizer_poll(
            &track->lanes[
              index_lane
            ].synthesizer
          );
        }

        buffer_out[index_buffer_out] = math_c_floating_point_minimum(
          math_c_floating_point_maximum((
              value / (float) track->length_lanes
            ),
            -1.0f
          ),
          1.0f
        );
      } else {
        buffer_out[index_buffer_out] = (
          buffer_out[index_buffer_out - channel]
        );
      }

      track->progress = (
        ((float) io_proc_data->frame) / 
        (((float) track->length) * 100.0f)
      );

      if (
        channel == 0 &&
        io_proc_data->frame == track->length * 100
      ) {
        io_proc_data->frame = 0;

        track->complete = 1;

        track = track_upcoming;

        pthread_mutex_unlock(
          &queue_mutex_track_completion
        );
      }
    }
  }

  display_render();

  return 0;
}
