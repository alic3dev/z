#include <z_close_exit.h>

pthread_mutex_t z_close_exit_mutex;

void z_close_exit_initialize() {
  pthread_mutex_init(
    &z_close_exit_mutex,
    (void*)0
  );

  pthread_mutex_lock(
    &z_close_exit_mutex
  );
}

void z_close_exit() {
  pthread_mutex_unlock(
    &z_close_exit_mutex
  );
}
