#include <io_proc_data.h>

#include <cer0.h>

void io_proc_data_initialize(
  struct io_proc_data* io_proc_data
) {
  io_proc_data->frame = 0;

  io_proc_data->note_table = (void*)0;
  io_proc_data->length_note_table = 0; 
  io_proc_data->steps_notes = 5;

  io_proc_data->z = 0;

  io_proc_data->octave_starting = 1;
  io_proc_data->octave_ending = 4;

  io_proc_data->note_table = cer0_note_table_stepped_create(
    io_proc_data->octave_starting,
    io_proc_data->octave_ending,
    cer0_frequency_root_standard,
    io_proc_data->steps_notes
  );

  io_proc_data->length_note_table = cer0_note_table_stepped_length(
    io_proc_data->octave_starting,
    io_proc_data->octave_ending,
    io_proc_data->steps_notes
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
      sine
    );
  }
}

void io_proc_data_destroy(
  struct io_proc_data* io_proc_data
) {
  free(io_proc_data->note_table);
  
  cer0_synthesizer_destroy(
    &io_proc_data->synthesizer
  );
}
