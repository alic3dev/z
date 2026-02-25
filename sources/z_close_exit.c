#include <z_close_exit.h>

#include <pthread.h>
#include <stdio.h>

void z_close_exit(
  int status,
  void* data
) {
  struct z_close_exit_data* z_close_exit_data = (
    data
  );

  pthread_mutex_unlock(
    &z_close_exit_data->mutex_initializer
  );

  pthread_mutex_unlock(
    &z_close_exit_data->mutex_close_exit
  );
}
