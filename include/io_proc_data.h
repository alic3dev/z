#ifndef __io_proc_data_h
#define __io_proc_data_h

#include <cer0.h>

struct io_proc_data {
  unsigned short int frame;
  unsigned char initialized;
  unsigned int length_note_table;
  float* note_table;
  signed char octave_ending;
  signed char octave_starting;
  signed char octave_range;
  unsigned char steps_notes;
  struct cer0_synthesizer synthesizer;
  unsigned long z;

  const unsigned char* scale;
  unsigned char length_scale;
};

void io_proc_data_initialize(
  struct io_proc_data*
);

void io_proc_data_initialize_synthesizer(
  struct io_proc_data*,
  float
);

void io_proc_data_destroy(
  struct io_proc_data*
);

#endif
