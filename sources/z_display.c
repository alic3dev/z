#include <z_display.h>

#include <z_queue.h>
#include <z_words.h>

#include <clic3_memory.h>

#include <math_c.h>

#include <stdio.h>
#include <sys/ioctl.h>

void z_display_render(
  struct z_queue* z_queue
) {
  struct winsize terminal_size;

  signed char status_ioctl = ioctl(
    0,
    TIOCGWINSZ,
    &terminal_size
  );

  char* buffer_track_progress;

  if (
    status_ioctl == -1 ||
    terminal_size.ws_col < 1
  ) {
    buffer_track_progress = (
      clic3_memory_allocate_raw(
        0
      )
    );
  } else {
    unsigned int width = terminal_size.ws_col;

    buffer_track_progress = (
      clic3_memory_allocate_raw(
        width +
        1
      )
    );

    buffer_track_progress[0] = (
      '|'
    );
    
    buffer_track_progress[
      (width - 1)
    ] = (
      '|'
    );

    buffer_track_progress[
      width
    ] = (
      '\0'
    );

    unsigned int length_progress_indicators = (
      (
        width -
        1
      ) *
      math_c_floating_point_minimum(
        math_c_floating_point_maximum(
          z_queue->track_current->progress,
          0.0f
        ),
        1.0f
      )
    );

    for (
      unsigned int index_buffer_track_progress = 1;
      index_buffer_track_progress < width - 1;
      ++index_buffer_track_progress
    ) {
      if (
        index_buffer_track_progress <= length_progress_indicators
      ) {
        if (
          index_buffer_track_progress != width - 2 &&
          index_buffer_track_progress == length_progress_indicators
        ) {
          buffer_track_progress[
            index_buffer_track_progress
          ] = (
            '>'
          );
        } else {
          buffer_track_progress[
            index_buffer_track_progress
          ] = (
            '-'
          );
        }
      } else {
        buffer_track_progress[
          index_buffer_track_progress
        ] = (
          ' '
        );
      }
    }
  }

  printf(
    "\e[?25l"
    "\e[H\e[2J\e[3J"
    "playing_track->{"
  );

  for (
    unsigned char index_name = 0;
    z_queue->track_current->name[
      index_name
    ] != '\0';
    ++index_name
  ) {
    printf(
      index_name != 0
      ? " %s"
      : "%s",
      z_words[
        z_queue->track_current->name[
          index_name
        ] %
        z_words_length
      ]
    );
  }
  
  printf(
    "}\n"
    "upcoming_track->{"
  );
  
  for (
    unsigned char index_name = 0;
    z_queue->track_upcoming->name[
      index_name
    ] != '\0';
    ++index_name
  ) {
    printf(
      index_name != 0
      ? " %s"
      : "%s",
      z_words[
        z_queue->track_upcoming->name[
          index_name
        ] %
        z_words_length
      ]
    );
  }

  printf(
    "}\n"
    "\n"
    // "seed: %s\n"
    "\n%s\n\n"
    ": "
    "\e[?25h",
    // z_queue->track_current->char_array_seed,
    buffer_track_progress
  );

  fflush(
    stdout
  );

  clic3_memory_free_raw(
    buffer_track_progress
  );
}

void z_display_render_event(
  void* z_event_data
) {
  struct z_queue* z_queue = (
    z_event_data
  );

  z_display_render(
    z_queue
  );
}
