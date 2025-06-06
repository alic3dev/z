#include <io_proc_data.h>

#include <cer0.h>

void io_proc_data_initialize(
  struct io_proc_data* io_proc_data
) {
  io_proc_data->frame = 0;
  io_proc_data->amplitude = 1.0f;

  io_proc_data->note_table = (void*)0;
  io_proc_data->length_note_table = 0; 
  io_proc_data->steps_notes = 5;

  io_proc_data->z = 0;

  io_proc_data->length_oscillators = 0;
  io_proc_data->oscillators = (void*)0;

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

  io_proc_data->length_oscillators = 0;
  io_proc_data->oscillators = (void*)0;
}

void io_proc_data_initialize_oscillators(
  struct io_proc_data* io_proc_data,
  float sample_rate
) {
  io_proc_data->length_oscillators = 4;
  io_proc_data->oscillators = malloc(
    sizeof(struct cer0_oscillator) * io_proc_data->length_oscillators
  );

  for (
    unsigned int index_oscillator = 0;
    index_oscillator < io_proc_data->length_oscillators;
    ++index_oscillator
  ) {
    cer0_oscillator_initialize(
      &io_proc_data->oscillators[index_oscillator],
      sample_rate,
      0,
      sine
    );
  }
}

void io_proc_data_destroy(
  struct io_proc_data* io_proc_data
) {
  free(io_proc_data->note_table);
  free(io_proc_data->oscillators);
}
