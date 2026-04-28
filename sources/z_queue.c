#include <z_queue.h>

#include <z_display.h>
#include <z_event.h>
#include <z_queue_status.h>
#include <z_track.h>

#include <clic3_memory.h>

void z_queue_initialize(
  struct z_queue* z_queue,
  struct z_track_parameters* z_track_parameters,
  float* rate_sample
) {
  z_queue->index_track = (
    0x00
  );

  z_queue->rate_sample = (
    rate_sample
  );

  z_queue->track_current = (
    clic3_memory_allocate_raw(
      sizeof(
        struct z_track
      )
    )
  );

  z_queue->track_upcoming = (
    clic3_memory_allocate_raw(
      sizeof(
        struct z_track
      )
    )
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

void z_queue_track_next(
  struct z_queue* z_queue
) {
  struct z_track* track_previous = (
    z_queue->track_current
  );

  z_queue->track_current = (
    z_queue->track_upcoming
  );

  z_track_destroy(
    track_previous
  );

  clic3_memory_free_raw(
    track_previous
  );

  z_queue->track_upcoming = (
    clic3_memory_allocate_raw(
      sizeof(
        struct z_track
      )
    )
  );

  z_queue->index_track = (
    z_queue->index_track +
    1
  );

  z_event_trigger(
    z_event_type_track_pregeneration,
    z_queue
  );

  z_track_generate(
    z_queue->track_upcoming,
    z_queue->track_parameters,
    *z_queue->rate_sample
  );

  z_event_trigger(
    z_event_type_track_update,
    z_queue
  );
}

void z_queue_destroy(
  struct z_queue* z_queue
) {
  z_queue->status = (
    z_queue_status_exiting
  );

  z_track_destroy(
    z_queue->track_current
  );

  z_track_destroy(
    z_queue->track_upcoming
  );

  clic3_memory_free_raw(
    z_queue->track_current
  );

  clic3_memory_free_raw(
    z_queue->track_upcoming
  );
}
