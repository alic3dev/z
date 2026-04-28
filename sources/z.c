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

#include <clic3_memory.h>

#include <interrupt_handler.h>

#include <math_c_minimum.h>

#include <wave_chunk_data.h>
#include <wave_chunk_fact.h>
#include <wave_chunk_fmt.h>
#include <wave_chunk_riff.h>
#include <wave_format.h>
#include <wave_parameters.h>

#include <CoreAudio/CoreAudio.h>

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
    FILE* output = (
      fopen(
        parameters[
          0x01
        ],
        "wb"
      )
    );

    if (
      output ==
      0x00
    ) {
      fprintf(
        stderr,
        "failed_to_open->{%s};\n",
        parameters[
          0x01
        ]
      );

      return (
        0x01
      );
    }

    static struct z_track_parameters z_track_parameters;
    static struct z_io_proc_data z_io_proc_data;

    struct wave_parameters wave_parameters = {
      .wave_format = (
        wave_format_microsoft_pcm_format
      ),
      .length_channels = (
        0x01
      ),
      .rate_samples = (
        0xac44
      ),
      .bytes_sample = (
        0x02
      ),
      .length_samples = (
        0x4affff
      )
    };

    struct wave_chunk_data chunk_data;
    struct wave_chunk_fmt chunk_fmt;
    struct wave_chunk_riff chunk_riff;

    wave_chunk_riff_initialize(
      &chunk_riff,
      &wave_parameters
    );

    wave_chunk_fmt_initialize(
      &chunk_fmt,
      &wave_parameters
    );

    wave_chunk_data_initialize(
      &chunk_data,
      &wave_parameters
    );

    z_track_parameters_initialize_defaults(
      &z_track_parameters
    );

    float rate_samples = (
      wave_parameters.rate_samples *
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

    unsigned char* bytes = (
      clic3_memory_allocate_raw(
        wave_parameters.bytes_sample
      )
    );

    float* buffer = (
      clic3_memory_allocate_raw(
        wave_parameters.length_samples *
        wave_parameters.length_channels *
        sizeof(
          float
        )
      )
    );

    unsigned long int maximum_bytes = (
      0x00
    );

    for (
      unsigned char index_byte = (
        0x00
      );
      (
        index_byte <
        wave_parameters.bytes_sample
      );
      ++index_byte
    ) {
      maximum_bytes = (
        (
          maximum_bytes <<
          0x08
        ) +
        0xff
      );
    }

    for (
      unsigned long long int frame = (
        0x00
      );
      (
        frame <
        wave_parameters.length_samples *
        wave_parameters.length_channels
      );
      ++frame
    ) {
      unsigned long int index_data = (
        frame *
        wave_parameters.bytes_sample
      );

      unsigned long int value = (
        0x00
      );

      unsigned long int index = (
        frame /
        wave_parameters.length_channels
      );

      if (
        (
          frame %
          wave_parameters.length_channels
        ) ==
        0x00
      ){
        z_io_proc_frame_get(
          &z_io_proc_data,
          &z_io_proc_data.queue,
          buffer,
          index,
          0x00
        );

        value = (
          (
            buffer[
              index
            ] +
            0x01
          ) /
          0x02 *
          maximum_bytes
        );
      } else {
        value = (
          (
            buffer[
              index
            ] +
            0x01
          ) /
          0x02 *
          maximum_bytes
        );
      }

      value = (
        value /
        0x02
      );

      for (
        unsigned int index_byte = (
          0x00
        );
        (
          index_byte <
          wave_parameters.bytes_sample
        );
        ++index_byte
      ) {
        chunk_data.data[
          index_data +
          index_byte
        ] = (
          (
            (unsigned char*)
            &value
          )[
            index_byte
          ]
        );
      }
    }

    clic3_memory_free_raw(
      bytes
    );

    wave_chunk_riff_write(
      &chunk_riff,
      output
    );

    wave_chunk_fmt_write(
      &chunk_fmt,
      output
    );

    wave_chunk_data_write(
      &chunk_data,
      output
    );

    fclose(
      output
    );

    wave_chunk_riff_destroy(
      &chunk_riff
    );

    wave_chunk_fmt_destroy(
      &chunk_fmt
    );

    wave_chunk_data_destroy(
      &chunk_data
    );

    z_queue_destroy(
      &z_io_proc_data.queue
    );

    z_track_parameters_destroy(
      &z_track_parameters
    );

    return (
      0x00
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
