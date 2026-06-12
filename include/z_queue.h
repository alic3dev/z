#ifndef __z_queue_h
#define __z_queue_h

#include <z_event.h>
#include <z_track.h>
#include <z_track_parameters.h>
#include <z_queue_status.h>

#include <pthread.h>

struct z_queue {
  unsigned int index_track;

  struct z_track* track_current;
  struct z_track* track_upcoming;

  struct z_track_parameters* track_parameters;
  
  struct z_event_store event_store;

  enum z_queue_status status;

  float* rate_sample;
};

void z_queue_initialize(
  struct z_queue*,
  struct z_track_parameters*,
  float*
);

void z_queue_track_next(
  struct z_queue*
);

void z_queue_destroy(
  struct z_queue*
);

#endif
