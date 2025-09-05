#include <track_threads.h>

#include <status.h>
#include <track.h>

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t track_threads_mutex_track_completion;
pthread_mutex_t track_threads_mutex_track_generating;
pthread_t track_threads_thread_completion_handler;

void track_threads_initialize() {
  pthread_mutex_init(
    &track_threads_mutex_track_completion,
    (void*)0
  );

  pthread_mutex_init(
    &track_threads_mutex_track_generating,
    (void*)0
  );

  pthread_mutex_lock(
    &track_threads_mutex_track_completion
  );

  pthread_create(
    &track_threads_thread_completion_handler,
    (void*)0,
    track_threads_completion_handler,
    (void*)0
  );
}

void* track_threads_completion_handler(void* _) {
  while (status.playing == 1) {
    pthread_mutex_lock(
      &track_threads_mutex_track_completion
    );

    if (status.playing == 0) {
      break;
    }

    pthread_mutex_lock(
      &track_threads_mutex_track_generating
    );

    track_destroy(
      track_current
    );

    track_current = track_upcoming;

    track_upcoming = malloc(
      sizeof(struct track)
    );
    track_generate(track_upcoming);
    
    printf(
      "playing_track->{%s}\n",
      track_current->name
    );

    printf("upcoming_track->{%s}\n", track_upcoming->name);

    pthread_mutex_unlock(
      &track_threads_mutex_track_generating
    );
  }

  return (void*)0;
}

void track_threads_destroy() {
  pthread_mutex_destroy(
    &track_threads_mutex_track_completion
  );

  pthread_join(
    track_threads_thread_completion_handler,
    (void*)0
  );
}
