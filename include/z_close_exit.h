#ifndef __z_close_exit_h
#define __z_close_exit_h

#include <pthread.h>

struct z_close_exit_data {
  pthread_mutex_t mutex_close_exit;
  pthread_mutex_t mutex_initializer;

  unsigned int initializing;
};

void z_close_exit(
  int,
  void*
);

#endif
