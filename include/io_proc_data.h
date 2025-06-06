#ifndef __io_proc_data_h
#define __io_proc_data_h

struct io_proc_data {
  float amplitude;
  unsigned short int frame;
  unsigned int length_note_table;
  unsigned int length_oscillators;
  float* note_table;
  signed char octave_ending;
  signed char octave_starting;
  struct cer0_oscillator* oscillators;
  unsigned char steps_notes;
  unsigned long z;
};

void io_proc_data_initialize(
  struct io_proc_data*
);

void io_proc_data_initialize_oscillators(
  struct io_proc_data*,
  float
);

void io_proc_data_destroy(
  struct io_proc_data*
);

#endif
