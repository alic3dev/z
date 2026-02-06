#ifndef __z_display_h
#define __z_display_h

#include <z_queue.h>

struct z_display_data {
  char* name;
  char* name_next;
  char* progress;
  char* seed;
};

void z_display_render(
  struct z_display_data*
);

void z_display_data_initialize(
  struct z_display_data*
);

void z_display_data_set(
  struct z_display_data* z_display_data,
  struct z_queue* z_queue
);

void z_display_data_destroy(
  struct z_display_data*
);

#endif
