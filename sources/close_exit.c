#include <close_exit.h>

#include <status.h>
#include <track_threads.h>

pthread_mutex_t close_exit_mutex;

void close_exit_initialize() {
  pthread_mutex_init(
    &close_exit_mutex,
    (void*)0
  );

  pthread_mutex_lock(
    &close_exit_mutex
  );
}

void close_exit() {
  status.playing = 0;
  
  pthread_mutex_unlock(
    &track_threads_mutex_track_completion
  );

  pthread_mutex_unlock(
    &close_exit_mutex
  );
}
