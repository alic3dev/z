#include <queue.h>

// #include <display.h>
#include <status.h>
#include <track.h>

#include <pthread.h>
#include <stdlib.h>

struct track* track_current = (void*)0;
struct track* track_upcoming = (void*)0;

pthread_mutex_t queue_mutex_track_completion;
pthread_mutex_t queue_mutex_track_generating;
pthread_t queue_thread_track_completion_handler;

void queue_initialize() {
  track_current = malloc(
    sizeof(struct track)
  );

  track_upcoming = malloc(
    sizeof(struct track)
  );

  track_generate(
    track_current
  );

  track_generate(
    track_upcoming
  );

  display_render();

  pthread_mutex_init(
    &queue_mutex_track_completion,
    (void*)0
  );

  pthread_mutex_init(
    &queue_mutex_track_generating,
    (void*)0
  );

  pthread_mutex_lock(
    &queue_mutex_track_completion
  );

  pthread_create(
    &queue_thread_track_completion_handler,
    (void*)0,
    queue_track_completion_handler,
    (void*)0
  );
}

void* queue_track_completion_handler(void* _) {
  while (status.playing == 1) {
    pthread_mutex_lock(
      &queue_mutex_track_completion
    );

    if (status.playing == 0) {
      break;
    }

    pthread_mutex_lock(
      &queue_mutex_track_generating
    );

    track_destroy(
      track_current
    );

    track_current = track_upcoming;

    track_upcoming = malloc(
      sizeof(struct track)
    );
    track_generate(track_upcoming);
    
    display_render();

    pthread_mutex_unlock(
      &queue_mutex_track_generating
    );
  }

  return (void*)0;
}

void queue_destroy() {
  pthread_mutex_destroy(
    &queue_mutex_track_completion
  );

  pthread_join(
    queue_thread_track_completion_handler,
    (void*)0
  );
}
