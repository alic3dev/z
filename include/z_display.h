#ifndef __z_display_h
#define __z_display_h

#include <z_queue.h>

void z_display_render(
  struct z_queue*
);

void z_display_render_event(
  void* z_event_data
);

#endif
