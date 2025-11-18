#ifndef __z_queue_h
#define __z_queue_h

#include <z_track.h>
#include <z_queue_status.h>

#include <cer0_audio_output.h>

#include <pthread.h>

struct z_queue {
  struct cer0_audio_output* audio_output;

  struct z_track* track_current;
  struct z_track* track_upcoming;

  enum z_queue_status status;
};

void z_queue_initialize(
  struct z_queue*,
  struct cer0_audio_output*
);

void z_queue_destroy(
  struct z_queue*
);

#endif
