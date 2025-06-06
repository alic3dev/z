#include <z.h>
#include <io_proc_data.h>

#include <cer0.h>

#include <CoreAudio/CoreAudio.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    io_proc_data->length_oscillators > 0
  ) {
    if (rand() % 128 == 0) {
      cer0_oscillator_frequency_set(
        &io_proc_data->oscillators[0],
        io_proc_data->note_table[
          (rand() % io_proc_data->steps_notes) +
          io_proc_data->steps_notes
        ]
      );

      cer0_oscillator_frequency_set(
        &io_proc_data->oscillators[1],
        io_proc_data->oscillators[0].phase.frequency
      );
    }

    if (
      io_proc_data->frame % 8 == 0
    ) {
      cer0_oscillator_frequency_set(
        &io_proc_data->oscillators[2],
        io_proc_data->note_table[
          ((io_proc_data->z % 2 == 0
            ? io_proc_data->z % 3 == 0 
            ? io_proc_data->z - 1 
            : io_proc_data->z 
            : io_proc_data->z + 1
          ) % (io_proc_data->steps_notes)) + 
          (io_proc_data->length_note_table / 2) +
          (io_proc_data->frame % 31 == 0
            ? io_proc_data->steps_notes
            : 0
          ) - 1
        ]
      );

      cer0_oscillator_frequency_set(
        &io_proc_data->oscillators[3],
        io_proc_data->oscillators[2].phase.frequency
      );

      io_proc_data->z = (
        io_proc_data->z + 1
      );
    }
  }

  io_proc_data->frame = (
    io_proc_data->frame + 1
  );

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
      unsigned long int channel = index_buffer % count_channel_out;

      if (channel == 0) {
        float value_buffer_out = 0.0f;

        for (
          unsigned int index_oscillator = 0;
          index_oscillator < io_proc_data->length_oscillators;
          ++index_oscillator
        ) {
          value_buffer_out = (
            value_buffer_out +
            (cer0_oscillator_poll(
              &io_proc_data->oscillators[index_oscillator]
            ) / io_proc_data->length_oscillators)
          );
        }

        buffer_out[index_buffer_out] = value_buffer_out * io_proc_data->amplitude;
      } else {
        buffer_out[index_buffer_out] = ((float*) buffer_list_audio_out->mBuffers[0].mData)[index_buffer_out];
      }
    }
  }

  return 0;
}

int main() {
  srand(time((void*)0));

  struct io_proc_data io_proc_data;

  io_proc_data_initialize(
    &io_proc_data
  );

  struct cer0_audio_output audio_output;
  cer0_audio_output_initialize(
    &audio_output,
    io_proc,
    &io_proc_data
  );

  io_proc_data_initialize_oscillators(
    &io_proc_data,
    audio_output.sample_rate
  );

  getc(stdin);

  cer0_audio_output_destroy(
    &audio_output
  );

  io_proc_data_destroy(
    &io_proc_data
  );

  return 0;
}
