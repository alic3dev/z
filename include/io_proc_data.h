#ifndef __io_proc_data_h
#define __io_proc_data_h

#include <track.h>

#include <cer0.h>

struct io_proc_data {
  unsigned long long int frame;
  unsigned char initialized;
};

void io_proc_data_initialize(
  struct io_proc_data*
);

#endif
