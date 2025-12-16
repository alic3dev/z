#include <z.h>

#include <z_close_exit.h>
#include <z_display.h>
#include <z_event.h>
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
  struct z_io_proc_data z_io_proc_data;

  z_close_exit_initialize();

  interrupt_handler_initialize();

  interrupt_handler_interrupt_function_add(
    z_close_exit
  );

  struct cer0_audio_output audio_output;

  z_io_proc_data_initialize(
    &z_io_proc_data,
    &audio_output.sample_rate
  );

  z_event_initialize();

  z_event_on(
    z_display_render_event,
    z_event_type_track_update
  );

  cer0_audio_output_initialize(
    &audio_output,
    z_io_proc,
    &z_io_proc_data
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
  
  return 0;
}
