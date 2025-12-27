#include <z_queue.h>

#include <z_display.h>
#include <z_queue_status.h>
#include <z_track.h>

#include <stdlib.h>

void z_queue_initialize(
  struct z_queue* z_queue,
  struct z_track_parameters* z_track_parameters,
  float* rate_sample
) {
  z_queue->rate_sample = rate_sample;

  z_queue->track_current = malloc(
    sizeof(struct z_track)
  );

  z_queue->track_upcoming = malloc(
    sizeof(struct z_track)
  );

  z_queue->track_parameters = (
    z_track_parameters
  );

  z_track_generate(
    z_queue->track_current,
    z_queue->track_parameters,
    *z_queue->rate_sample
  );

  z_track_generate(
    z_queue->track_upcoming,
    z_queue->track_parameters,
    *z_queue->rate_sample
  );
}

void z_queue_destroy(
  struct z_queue* z_queue
) {
  z_queue->status = z_queue_status_exiting;

  z_track_destroy(
    z_queue->track_current
  );

  z_track_destroy(
    z_queue->track_upcoming
  );

  free(
    z_queue->track_current
  );
  
  free(
    z_queue->track_upcoming
  );
}
