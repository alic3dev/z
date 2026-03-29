#include <z_display.h>

#include <z_queue.h>
#include <z_words.h>

#include <clic3_bytes.h>

#include <clic3_memory.h>

#include <stdio.h>

void z_display_render(
  struct z_display_data* z_display_data
) {
  printf(
    "\e[?25l"
    "\e[H\e[2J\e[3J"
    "playing_track->{"
    "%s"
    "}\n"
    "upcoming_track->{"
    "%s"
    "}\n"
    "\n"
    // "seed: %s\n"
    "\n%s\n\n"
    ": "
    "\e[?25h",
    z_display_data->name,
    z_display_data->name_next,
    // z_display_data->seed,
    z_display_data->progress
  );

  fflush(
    stdout
  );
}

void z_display_data_initialize(
  struct z_display_data* z_display_data
) {
  z_display_data->name = (
    clic3_memory_allocate_raw(
      1
    )
  );

  z_display_data->name_next = (
    clic3_memory_allocate_raw(
      1
    )
  );

  z_display_data->seed = (
    clic3_memory_allocate_raw(
      1
    )
  );

  z_display_data->progress = (
    clic3_memory_allocate_raw(
      1
    )
  );

  z_display_data->name[
    0
  ] = (
    '\0'
  );

  z_display_data->name_next[
    0
  ] = (
    '\0'
  );

  z_display_data->seed[
    0
  ] = (
    '\0'
  );

  z_display_data->progress[
    0
  ] = (
    '\0'
  );
}

void z_display_data_set(
  struct z_display_data* z_display_data,
  struct z_queue* z_queue
) {
  clic3_memory_reallocate_raw(
    &z_display_data->seed,
    (
      z_queue->track_current->length_char_array_seed +
      1
    )
  );

  clic3_bytes_copy(
    z_display_data->seed,
    z_queue->track_current->char_array_seed,
    (
      z_queue->track_current->length_char_array_seed +
      1
    )
  );

  z_words_construct(
    &z_display_data->name,
    z_queue->track_current->name
  );

  z_words_construct(
    &z_display_data->name_next,
    z_queue->track_upcoming->name
  );
}

void z_display_data_destroy(
  struct z_display_data* z_display_data
) {
  clic3_memory_free_raw(
    z_display_data->name
  );

  clic3_memory_free_raw(
    z_display_data->name_next
  );

  clic3_memory_free_raw(
    z_display_data->progress
  );

  clic3_memory_free_raw(
    z_display_data->seed
  );
}
