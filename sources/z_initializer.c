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
    0x00
  );

  struct timespec timespec_sleep = {
    .tv_sec = (
      0x00
    ),
    .tv_nsec = (
      0x0ee6b280
    )
  };

  struct timespec timespec_sleep_remaining;

  while (
    *initializing ==
    0x01
  ) {
    printf(
      "\e[H\e[2J\e[3J"
      "z::initializing"
    );

    for (
      unsigned char index_dot = (
        0x00
      );
      (
        index_dot <
        length_dots
      );
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
        0x01
      ) %
      0x04
    );

    nanosleep(
      &timespec_sleep,
      &timespec_sleep_remaining
    );
  }

  return (
    0x00
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
    0x00
  );
}
