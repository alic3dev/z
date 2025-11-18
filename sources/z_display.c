#include <z_display.h>

#include <z_queue.h>

#include <math_c.h>

#include <stdlib.h>
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
    buffer_track_progress = malloc(0);
  } else {
    unsigned int width = terminal_size.ws_col;

    buffer_track_progress = malloc(
      sizeof(char) * (
        width + 1
      )
    );

    buffer_track_progress[0] = '|';
    
    buffer_track_progress[
      (width - 1)
    ] = '|';

    buffer_track_progress[
      width
    ] = '\0';

    unsigned int length_progress_indicators = (
      (width - 1) * math_c_floating_point_minimum(
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
          ] = '>';
        } else {
          buffer_track_progress[
            index_buffer_track_progress
          ] = '-';
        }
      } else {
        buffer_track_progress[
          index_buffer_track_progress
        ] = ' ';
      }
    }
  }

  printf(
    "\e[?25l"
    "\e[H\e[2J\e[3J"
    "playing_track->{%s}\n"
    "upcoming_track->{%s}\n"
    "\n"
    "seed: %.2x %.2x %.2x %.2x\n"
    "\n"
    "%s\n"
    "\n"
    ": "
    "\e[?25h",
    z_queue->track_current->name,
    z_queue->track_upcoming->name,
    z_queue->track_current->buffer_seed[0],
    z_queue->track_current->buffer_seed[1],
    z_queue->track_current->buffer_seed[2],
    z_queue->track_current->buffer_seed[3],
    buffer_track_progress
  );

  fflush(stdout);

  free(buffer_track_progress);
}
