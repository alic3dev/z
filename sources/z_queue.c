#include <z_queue.h>

#include <z_display.h>
#include <z_queue_status.h>
#include <z_track.h>

#include <cer0_audio_output.h>

#include <pthread.h>
#include <stdlib.h>

void z_queue_initialize(
  struct z_queue* z_queue,
  struct cer0_audio_output* audio_output
) {
  z_queue->audio_output = audio_output;

  z_queue->track_current = malloc(
    sizeof(struct z_track)
  );

  z_queue->track_upcoming = malloc(
    sizeof(struct z_track)
  );

  z_track_generate(
    z_queue->track_current,
    z_queue->audio_output->sample_rate
  );

  z_track_generate(
    z_queue->track_upcoming,
    z_queue->audio_output->sample_rate
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
