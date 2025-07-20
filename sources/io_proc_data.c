#include <io_proc_data.h>

#include <cer0.h>

void io_proc_data_initialize(
  struct io_proc_data* io_proc_data
) {
  io_proc_data->frame = 0;
  
  io_proc_data->initialized = 0;

  io_proc_data->note_table = (void*)0;
  io_proc_data->length_note_table = 0; 
  io_proc_data->steps_notes = cer0_steps_in_octave;

  io_proc_data->z = 0;

  io_proc_data->octave_starting = 0;
  io_proc_data->octave_ending = 10;
  io_proc_data->octave_range = (
    io_proc_data->octave_ending -
    io_proc_data->octave_starting
  );

  io_proc_data->note_table = cer0_note_table_create(
    io_proc_data->octave_starting,
    io_proc_data->octave_ending,
    cer0_frequency_root_standard
  );

  io_proc_data->length_note_table = cer0_note_table_length(
    io_proc_data->octave_starting,
    io_proc_data->octave_ending
  );

  io_proc_data->scale = (
    cer0_scale_notes_minor_pentatonic
  );
  io_proc_data->length_scale = (
    cer0_scale_length_minor_pentatonic
  );
}

void io_proc_data_initialize_synthesizer(
  struct io_proc_data* io_proc_data,
  float sample_rate
) {
  cer0_synthesizer_initialize(
    &io_proc_data->synthesizer,
    sample_rate
  );

  for (
    unsigned int index_oscillator = 0;
    index_oscillator < 4;
    ++index_oscillator
  ) {
    cer0_synthesizer_oscillator_add(
      &io_proc_data->synthesizer,
      index_oscillator == 0
      ? sine
      : index_oscillator == 1
      ? square
      : triangle
    );
  }

  io_proc_data->initialized = 1;
}

void io_proc_data_destroy(
  struct io_proc_data* io_proc_data
) {
  free(io_proc_data->note_table);
  
  cer0_synthesizer_destroy(
    &io_proc_data->synthesizer
  );
}
