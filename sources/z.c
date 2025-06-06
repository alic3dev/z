#include <cer0.h>

#include <CoreAudio/CoreAudio.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

unsigned short int frame = 0;

float amplitude = 1.0f;

float* note_table = (void*)0;
unsigned int length_note_table = 0; 
unsigned char steps_notes = 5;

unsigned long int z = 0;

unsigned int length_oscillators = 0;
struct cer0_oscillator* oscillators = (void*)0;

OSStatus io_proc(
  AudioObjectID id_audio_object,
  const AudioTimeStamp* timestamp_audio,
  const AudioBufferList* buffer_list_audio_in,
  const AudioTimeStamp* timestamp_audio_in,
  AudioBufferList* buffer_list_audio_out,
  const AudioTimeStamp* timestamp_audio_out,
  void* data
) {
  if (length_oscillators > 0) {
    if (rand() % 128 == 0) {
      cer0_oscillator_frequency_set(
        &oscillators[0],
        note_table[(rand() % steps_notes) + steps_notes]
      );

      cer0_oscillator_frequency_set(
        &oscillators[1],
        oscillators[0].phase.frequency
      );
    }

    if (frame % 8 == 0) {
      cer0_oscillator_frequency_set(
        &oscillators[2],
        note_table[
          ((z % 2 == 0 ? z % 3 == 0 ? z - 1 : z : z + 1) % (steps_notes)) + 
          (length_note_table / 2) + (frame % 31 == 0 ? steps_notes : 0) - 1
        ]
      );

      cer0_oscillator_frequency_set(
        &oscillators[3],
        oscillators[2].phase.frequency
      );

      z = z + 1;
    }
  }

  frame = (
    frame + 1
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
          index_oscillator < length_oscillators;
          ++index_oscillator
        ) {
          value_buffer_out = (
            value_buffer_out +
            (cer0_oscillator_poll(
              &oscillators[index_oscillator]
            ) / length_oscillators)
          );
        }

        buffer_out[index_buffer_out] = value_buffer_out * amplitude;
      } else {
        buffer_out[index_buffer_out] = ((float*) buffer_list_audio_out->mBuffers[0].mData)[index_buffer_out];
      }
    }
  }

  return 0;
}

int main() {
  srand(time((void*)0));

  char octave_starting = 1;
  char octave_ending = 4;

  note_table = cer0_note_table_stepped_create(
    octave_starting,
    octave_ending,
    cer0_frequency_root_standard,
    steps_notes
  );

  length_note_table = cer0_note_table_stepped_length(
    octave_starting,
    octave_ending,
    steps_notes
  );

  struct cer0_audio_output audio_output;
  cer0_audio_output_initialize(
    &audio_output,
    io_proc,
    (void*)0
  );

  length_oscillators = 4;
  oscillators = malloc(
    sizeof(struct cer0_oscillator) * length_oscillators
  );

  for (
    unsigned int index_oscillator = 0;
    index_oscillator < length_oscillators;
    ++index_oscillator
  ) {
    cer0_oscillator_initialize(
      &oscillators[index_oscillator],
      audio_output.sample_rate,
      0,
      sine
    );
  }

  getc(stdin);

  cer0_audio_output_destroy(
    &audio_output
  );

  free(note_table);
  free(oscillators);

  return 0;
}
