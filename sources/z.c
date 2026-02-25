#include <z.h>

#include <z_close_exit.h>
#include <z_display_thread.h>
#include <z_event.h>
#include <z_initializer.h>
#include <z_io_proc.h>
#include <z_io_proc_data.h>
#include <z_queue.h>
#include <z_settings.h>
#include <z_track.h>

#include <cer0.h>
#include <interrupt_handler.h>

#include <CoreAudio/CoreAudio.h>

#include <pthread.h>

int main() {
  unsigned char initializing = (
    1
  );

  pthread_t thread_initializer;
  pthread_t thread_initializer_display;

  pthread_create(
    &thread_initializer_display,
    0,
    z_initializer_thread_display,
    &initializing
  );

  pthread_create(
    &thread_initializer,
    0,
    z_initializer_thread,
    0
  );

  pthread_join(
    thread_initializer,
    0
  );

  struct cer0_audio_output audio_output;
  struct z_display_thread_data z_display_thread_data;
  struct z_io_proc_data z_io_proc_data;
  struct z_track_parameters z_track_parameters;

  z_close_exit_initialize();

  interrupt_handler_initialize();

  interrupt_handler_interrupt_function_add(
    z_close_exit
  );

  z_track_parameters_initialize_defaults(
    &z_track_parameters
  );

  z_io_proc_data_initialize(
    &z_io_proc_data,
    &z_track_parameters,
    &audio_output.sample_rate
  );

  z_event_initialize();

  z_display_thread_initialize(
    &z_display_thread_data,
    &z_io_proc_data.queue
  );

  cer0_audio_output_initialize(
    &audio_output,
    z_io_proc,
    &z_io_proc_data
  );

  initializing = (
    0
  );

  pthread_join(
    thread_initializer_display,
    0
  );

  pthread_mutex_lock(
    &z_close_exit_mutex
  );

  z_io_proc_data.exiting = 1;

  pthread_mutex_lock(
    &z_io_proc_data.mutex_exited
  );

  cer0_audio_output_destroy(
    &audio_output
  );

  pthread_mutex_destroy(
    &z_io_proc_data.mutex_exited
  );

  pthread_mutex_destroy(
    &z_io_proc_data.mutex_exited
  );

  z_event_destroy();

  z_display_thread_destroy(
    &z_display_thread_data
  );

  z_queue_destroy(
    &z_io_proc_data.queue
  );

  z_track_parameters_destroy(
    &z_track_parameters
  );
  
  return 0;
}
