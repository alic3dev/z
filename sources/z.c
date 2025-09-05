#include <z.h>

#include <close_exit.h>
#include <io_proc.h>
#include <io_proc_data.h>
#include <queue.h>
#include <track.h>
#include <value.h>

#include <cer0.h>
#include <interrupt_handler.h>

#include <CoreAudio/CoreAudio.h>

#include <pthread.h>

int main() {
  struct io_proc_data io_proc_data;

  value_seed();

  queue_initialize();

  close_exit_initialize();

  interrupt_handler_initialize();

  interrupt_handler_interrupt_function_add(
    close_exit
  );

  io_proc_data_initialize(
    &io_proc_data
  );

  struct cer0_audio_output audio_output;
  cer0_audio_output_initialize(
    &audio_output,
    io_proc,
    &io_proc_data
  );

  io_proc_data.initialized = 1;

  pthread_mutex_lock(
    &close_exit_mutex
  );

  track_destroy(track_current);
  track_destroy(track_upcoming);

  queue_destroy();

  cer0_audio_output_destroy(
    &audio_output
  );

  return 0;
}
