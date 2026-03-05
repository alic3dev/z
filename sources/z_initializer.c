#include <z_initializer.h>

#include <cer0_signal.h>

#include <pthread.h>
#include <stdio.h>
#include <time.h>

void* z_initializer_thread_display(
  void* data
) {
  unsigned char* initializing = (
    data
  );

  unsigned char length_dots = (
    0
  );

  struct timespec timespec_sleep = {
    .tv_sec = 0,
    .tv_nsec = 250000000
  };

  struct timespec timespec_sleep_remaining;

  while (
    *initializing == 1
  ) {
    printf(
      "\e[H\e[2J\e[3J"
      "z::initializing"
    );

    for (
      unsigned char index_dot = 0;
      index_dot < length_dots;
      ++index_dot
    ) {
      printf(
        "."
      );
    }

    printf(
      "\n"
    );

    length_dots = (
      (
        length_dots +
        1
      ) %
      4
    );

    nanosleep(
      &timespec_sleep,
      &timespec_sleep_remaining
    );
  }

  return (
    0
  );
}

void* z_initializer_thread(
  void* data
) {
  pthread_mutex_t* mutex_initializer = (
    data
  );

  cer0_signal_sine_alice(
    333.333f
  );

  pthread_mutex_unlock(
    mutex_initializer
  );

  return (
    0
  );
}
