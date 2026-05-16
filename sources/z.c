#include <z.h>

#include <z_close_exit.h>
#include <z_display_thread.h>
#include <z_event.h>
#include <z_export.h>
#include <z_initializer.h>
#include <z_io_proc.h>
#include <z_io_proc_data.h>
#include <z_queue.h>
#include <z_settings.h>
#include <z_track.h>

#include <cer0.h>

#include <clic3_memory.h>

#include <interrupt_handler.h>

#include <pthread.h>
#include <stdio.h>

int main(
  int length_parameters,
  char** parameters
) {
  if (
    length_parameters ==
    0x02
  ) {
    static struct z_track_parameters z_track_parameters;
    static struct z_io_proc_data z_io_proc_data;

    z_track_parameters_initialize_defaults(
      &z_track_parameters
    );

    float rate_samples = (
      z_export_default_rate_samples *
      0x02
    );

    z_io_proc_data_initialize(
      &z_io_proc_data,
      &z_track_parameters,
      &rate_samples
    );

    z_queue_initialize(
      &z_io_proc_data.queue,
      z_io_proc_data.track_parameters,
      z_io_proc_data.rate_sample
    );

    unsigned char status_export = (
      z_export(
        parameters[
          0x01
        ],
        &z_io_proc_data
      )
    );
    
    if (
      status_export !=
      0x00
    ) {
      fprintf(
        stderr,
        "failed_to_export.to->{%s};\n",
        parameters[
          0x01
        ]
      );
    }
    
    z_queue_destroy(
      &z_io_proc_data.queue
    );

    z_track_parameters_destroy(
      &z_track_parameters
    );

    return (
      status_export
    );
  } else if (
    length_parameters !=
    0x01
  ) {
    fprintf(
      stderr,
      "unknown_parameters\n"
    );

    return (
      0x01
    );
  }

  pthread_t thread_initializer;
  pthread_t thread_initializer_display;

  struct z_close_exit_data z_close_exit_data = {
    .initializing = (
      0x01
    )
  };

  pthread_mutex_init(
    &z_close_exit_data.mutex_initializer,
    0x00
  );

  pthread_mutex_lock(
    &z_close_exit_data.mutex_initializer
  );

  pthread_mutex_init(
    &z_close_exit_data.mutex_close_exit,
    0x00
  );

  pthread_mutex_lock(
    &z_close_exit_data.mutex_close_exit
  );

  pthread_create(
    &thread_initializer_display,
    0x00,
    z_initializer_thread_display,
    &z_close_exit_data.initializing
  );

  pthread_create(
    &thread_initializer,
    0x00,
    z_initializer_thread,
    &z_close_exit_data.mutex_initializer
  );

  static struct cer0_audio_output audio_output;
  static struct z_display_thread_data z_display_thread_data;
  static struct z_io_proc_data z_io_proc_data;
  static struct z_track_parameters z_track_parameters;

  interrupt_handler_initialize();

  interrupt_handler_interrupt_function_add_with_data(
    z_close_exit,
    &z_close_exit_data
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

  pthread_mutex_lock(
    &z_close_exit_data.mutex_initializer
  );

  if (
    interrupt_handler_interrupted ==
    0x00
  ) {
    z_io_proc_data.z_close_exit_data = (
      &z_close_exit_data
    );

    cer0_audio_output_initialize(
      &audio_output,
      z_io_proc,
      &z_io_proc_data
    );

    pthread_mutex_lock(
      &z_close_exit_data.mutex_close_exit
    );

    z_io_proc_data.exiting = (
      0x01
    );

    pthread_mutex_lock(
      &z_io_proc_data.mutex_exited
    );

    cer0_audio_output_destroy(
      &audio_output
    );
  }

  pthread_mutex_destroy(
    &z_close_exit_data.mutex_close_exit
  );

  pthread_mutex_destroy(
    &z_close_exit_data.mutex_initializer
  );

  pthread_mutex_destroy(
    &z_io_proc_data.mutex_exited
  );

  pthread_mutex_destroy(
    &z_io_proc_data.mutex_playing
  );

  z_event_destroy();

  z_display_thread_destroy(
    &z_display_thread_data
  );

  if (
    z_io_proc_data.initialized !=
    0x00
  ) {
    z_queue_destroy(
      &z_io_proc_data.queue
    );
  }

  z_track_parameters_destroy(
    &z_track_parameters
  );

  return (
    0x00
  );
}
