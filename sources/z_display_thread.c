#include <z_display_thread.h>

#include <z_display.h>
#include <z_event.h>
#include <z_queue.h>

#include <clic3_memory.h>

#include <math_c_maximum.h>
#include <math_c_minimum.h>

#include <pthread.h>
#include <sys/ioctl.h>

void* z_display_render_thread(
  void* data
) {
  struct z_display_thread_data* z_display_thread_data = (
    data
  );

  while (
    z_display_thread_data->destroying ==
    0x00
  ) {
    pthread_mutex_lock(
      &z_display_thread_data->mutex_render
    );

    if (
      z_display_thread_data->destroying !=
      0x00
    ) {
      continue;
    }

    z_display_thread_data->progress = (
      z_display_thread_data->queue->track_current->progress
    );

    if (
      z_display_thread_data->queue->index_track !=
      z_display_thread_data->index_track
    ) {
      z_display_thread_data_set(
        z_display_thread_data,
        z_display_thread_data->queue
      );
    }

    struct winsize terminal_size;

    signed char status_ioctl = ioctl(
      0x00,
      TIOCGWINSZ,
      &terminal_size
    );

    if (
      (
        status_ioctl ==
        -0x01
      ) ||
      (
        terminal_size.ws_col <
        0x01
      )
    ) {
      clic3_memory_reallocate_raw(
        &z_display_thread_data->display_data.progress,
        0x01
      );

      z_display_thread_data->display_data.progress[
        0x00
      ] = (
        '\0'
      );
    } else {
      unsigned int width = (
        terminal_size.ws_col
      );

      clic3_memory_reallocate_raw(
        &z_display_thread_data->display_data.progress,
        (
          width +
          0x01
        )
      );

      z_display_thread_data->display_data.progress[
        0x00
      ] = (
        '|'
      );

      z_display_thread_data->display_data.progress[
        width -
        0x01
      ] = (
        '|'
      );

      z_display_thread_data->display_data.progress[
        width
      ] = (
        '\0'
      );

      unsigned int length_progress_indicators = (
        (
          width -
          0x01
        ) *
        math_c_minimum_float(
          math_c_maximum_float(
            z_display_thread_data->progress,
            0x00
          ),
          0x01
        )
      );

      for (
        unsigned int index_progress = (
          0x01
        );
        (
          index_progress <
          (
            width -
            0x01
          )
        );
        ++index_progress
      ) {
        if (
          index_progress <=
          length_progress_indicators
        ) {
          if (
            (
              index_progress !=
              (
                width -
                0x02
              )
            ) &&
            (
              index_progress ==
              length_progress_indicators
            )
          ) {
            z_display_thread_data->display_data.progress[
              index_progress
            ] = (
              '>'
            );
          } else {
            z_display_thread_data->display_data.progress[
              index_progress
            ] = (
              '-'
            );
          }
        } else {
          z_display_thread_data->display_data.progress[
            index_progress
          ] = (
            ' '
          );
        }
      }
    }

    z_display_render(
      &z_display_thread_data->display_data
    );
  }

  return (
    0x00
  );
}

void z_display_thread_initialize(
  struct z_display_thread_data* z_display_thread_data,
  struct z_queue* z_queue
) {
  pthread_mutex_init(
    &z_display_thread_data->mutex_render,
    0x00
  );

  pthread_mutex_lock(
    &z_display_thread_data->mutex_render
  );

  z_display_data_initialize(
    &z_display_thread_data->display_data
  );

  z_display_thread_data->index_track = (
    0x00
  );

  z_display_thread_data->destroying = (
    0x00
  );

  pthread_create(
    &z_display_thread_data->thread,
    0x00,
    z_display_render_thread,
    z_display_thread_data
  );

  z_event_on_with_data(
    z_display_thread_render_event,
    z_event_type_track_update,
    z_display_thread_data
  );
}

void z_display_thread_data_set(
  struct z_display_thread_data* z_display_thread_data,
  struct z_queue* z_queue
) {
  z_display_thread_data->index_track = (
    z_queue->index_track
  );

  z_display_data_set(
    &z_display_thread_data->display_data,
    z_queue
  );
}

void z_display_thread_render_event(
  void* z_event_data,
  void* z_event_structure_function_data
) {
  struct z_queue* z_queue = (
    z_event_data
  );

  struct z_display_thread_data* z_display_thread_data = (
    z_event_structure_function_data
  );

  if (
    z_display_thread_data->queue !=
    z_queue
  ) {
    z_display_thread_data->queue = (
      z_queue
    );
  }

  pthread_mutex_unlock(
    &z_display_thread_data->mutex_render
  );
}

void z_display_thread_destroy(
  struct z_display_thread_data* z_display_thread_data
) {
  z_display_thread_data->destroying = (
    0x01
  );

  pthread_mutex_unlock(
    &z_display_thread_data->mutex_render
  );

  pthread_join(
    z_display_thread_data->thread,
    0x00
  );

  pthread_mutex_destroy(
    &z_display_thread_data->mutex_render
  );

  z_display_data_destroy(
    &z_display_thread_data->display_data
  );
}
