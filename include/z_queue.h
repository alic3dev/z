#ifndef __z_queue_h
#define __z_queue_h

#include <z_track.h>
#include <z_queue_status.h>

#include <pthread.h>

struct z_queue {
  struct z_track* track_current;
  struct z_track* track_upcoming;
  
  enum z_queue_status status;

  float* rate_sample;
};

void z_queue_initialize(
  struct z_queue*,
  float*
);

void z_queue_destroy(
  struct z_queue*
);

#endif
