#ifndef __z_io_proc_data_h
#define __z_io_proc_data_h

#include <z_queue.h>
#include <z_track_parameters.h>
#include <z_settings.h>

#include <pthread.h>

struct z_io_proc_data {
  unsigned char exiting;

  unsigned long long int frame;

  unsigned char initialized;

  struct z_settings settings;

  struct z_queue queue;

  struct z_track_parameters* track_parameters;

  pthread_mutex_t mutex_exited;
  pthread_mutex_t mutex_playing;

  float* rate_sample;
};

void z_io_proc_data_initialize(
  struct z_io_proc_data*,
  struct z_track_parameters*,
  float*
);

#endif
