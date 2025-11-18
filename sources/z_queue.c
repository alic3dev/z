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

  pthread_mutex_init(
    &z_queue->mutex_track_completion,
    (void*)0
  );

  pthread_mutex_init(
    &z_queue->mutex_track_generating,
    (void*)0
  );

  pthread_mutex_init(
    &z_queue->mutex_track_generating,
    (void*)0
  );

  pthread_mutex_lock(
    &z_queue->mutex_track_completion
  );

  pthread_create(
    &z_queue->thread_track_completion_handler,
    (void*)0,
    z_queue_track_completion_handler,
    z_queue
  );
}

void* z_queue_track_completion_handler(
  void* data
) {
  struct z_queue* z_queue = data;

  while (
    z_queue->status == z_queue_status_playing
  ) {
    pthread_mutex_lock(
      &z_queue->mutex_track_completion
    );

    if (
      z_queue->status == z_queue_status_exiting
    ) {
      break;
    }

    pthread_mutex_lock(
      &z_queue->mutex_track_generating
    );

    z_track_destroy(
      z_queue->track_current
    );

    z_queue->track_current = (
      z_queue->track_upcoming
    );

    z_queue->track_upcoming = malloc(
      sizeof(struct z_track)
    );

    z_track_generate(
      z_queue->track_upcoming,
      z_queue->audio_output->sample_rate
    );
    
    z_display_render(
      z_queue
    );

    pthread_mutex_unlock(
      &z_queue->mutex_track_generating
    );
  }

  return (
    (void*)0
  );
}

void z_queue_destroy(
  struct z_queue* z_queue
) {
  z_queue->status = z_queue_status_exiting;

  pthread_mutex_unlock(
    &z_queue->mutex_track_completion
  );

  pthread_join(
    z_queue->thread_track_completion_handler,
    (void*)0
  );

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

  pthread_mutex_destroy(
    &z_queue->mutex_track_completion
  );

  pthread_mutex_destroy(
    &z_queue->mutex_track_generating
  );
}
