#ifndef __z_display_thread_h
#define __z_display_thread_h

#include <z_display.h>
#include <z_queue.h>

#include <pthread.h>

struct z_display_thread_data {
  unsigned int index_track;

  pthread_t thread;

  pthread_mutex_t mutex_render;

  float progress;

  struct z_display_data display_data;
  struct z_queue* queue;

  unsigned char destroying;
};

void* z_display_thread(
  void*
);

void z_display_thread_initialize(
  struct z_display_thread_data*,
  struct z_queue*
);

void z_display_thread_data_set(
  struct z_display_thread_data*,
  struct z_queue*
);

void z_display_thread_render_event(
  void*,
  void*
);

void z_display_thread_destroy(
  struct z_display_thread_data*
);

#endif
