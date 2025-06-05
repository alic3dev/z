#include <cer0.h>

#include <CoreAudio/CoreAudio.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

float increment_phase = 0.0f;
float value_phase = 0.0f;

float increment_phase_high = 0.0f;
float value_phase_high = 0.0f;

float amplitude = 1.0f;

unsigned short int frame = 0;

float sample_rate = 0.0f;

float* note_table = (void*)0;
unsigned int length_note_table = 0; 
unsigned char steps_notes = 5;

float high_low_ratio = 0.5f;

unsigned long int z = 0;

OSStatus io_proc(
  AudioObjectID id_audio_object,
  const AudioTimeStamp* timestamp_audio,
  const AudioBufferList* buffer_list_audio_in,
  const AudioTimeStamp* timestamp_audio_in,
  AudioBufferList* buffer_list_audio_out,
  const AudioTimeStamp* timestamp_audio_out,
  void* data
) {
  unsigned long int count_buffer = buffer_list_audio_out->mNumberBuffers;
  float value_signal;
  float value_signal_high;

  if (rand() % 128 == 0) {
    increment_phase = cer0_phase_get_increment(
      sample_rate,
      note_table[(rand() % steps_notes) + steps_notes]
    );
  }

  if (frame % 8 == 0) {
    increment_phase_high = cer0_phase_get_increment(
      sample_rate,
      note_table[(z % 2 == 0 ? z % 3 == 0 ? z - 1 : z : z + 1) % (steps_notes) + (length_note_table / 2) + (frame % 31 == 0 ? steps_notes : 0) - 1]
    );

    z = z + 1;
  }

  frame = (
    frame + 1
  );

  for (
    unsigned long int index_buffer = 0;
    index_buffer < count_buffer;
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
        value_phase = cer0_phase_advance(
          value_phase,
          increment_phase
        );
        value_signal = cer0_signal_sine(value_phase);
        float value_signal_secondary = cer0_signal_triangle(value_phase);

        value_phase_high = cer0_phase_advance(
          value_phase_high,
          increment_phase_high
        );
        value_signal_high = cer0_signal_triangle(value_phase_high);
        float value_signal_high_secondary = cer0_signal_sine(value_phase_high);

        buffer_out[index_buffer_out] = (
          (
            (((value_signal * 0.9) + (value_signal_secondary * 0.1)) * (1.0f - high_low_ratio)) + 
            (((value_signal_high * 0.3) + (value_signal_high_secondary * 0.7)) * high_low_ratio)
          ) * amplitude
        );
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

  sample_rate = audio_output.sample_rate;

  getc(stdin);

  cer0_audio_output_destroy(
    &audio_output
  );

  free(note_table);

  return 0;
}
