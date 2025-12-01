#include <z_settings.h>

#include <clic3_bytes.h>
#include <clic3_char_arrays.h>

#include <stdio.h>
#include <stdlib.h>

void z_settings_initialize(
  struct z_settings* settings
) {
  settings->volume = 1.0f;

  char* path_home = getenv(
    "HOME"
  );

  char* path_settings = clic3_char_arrays_concatenate(
    path_home,
    "/.config/z"
  );

  FILE* file_settings = fopen(
    path_settings,
    "r"
  );

  free(
    path_settings
  );

  if (
    file_settings != (void*)0
  ) {
    char character_file_settings = getc(
      file_settings
    );

    char* buffer_line_file_settings;
    unsigned int length_buffer_line_file_settings = 0;

    buffer_line_file_settings = malloc(0);

    unsigned int index_value_start = 0;

    while (
      feof(file_settings) == 0 &&
      character_file_settings != '\0'
    ) {
      length_buffer_line_file_settings = (
        length_buffer_line_file_settings +
        1
      );

      buffer_line_file_settings = realloc(
        buffer_line_file_settings,
        sizeof(char) *
        length_buffer_line_file_settings
      );

      buffer_line_file_settings[
        length_buffer_line_file_settings -
        1
      ] = character_file_settings;

      if (
        index_value_start == 0 &&
        character_file_settings == '-'
      ) {
        index_value_start = length_buffer_line_file_settings;
      } else if (
        character_file_settings == '\n' || 
        character_file_settings == '\0'
      ) {
        if (
          index_value_start == 0 ||
          length_buffer_line_file_settings < index_value_start + 6 ||
          buffer_line_file_settings[
            length_buffer_line_file_settings -
            2
          ] != ';' ||
          buffer_line_file_settings[
            length_buffer_line_file_settings -
            3
          ] != '}' ||
          buffer_line_file_settings[
            index_value_start +
            1
          ] != '{'
        ) {
          length_buffer_line_file_settings = 0;

          continue;
        }

        char* settings_name;
        char* settings_value;

        unsigned int length_settings_value = (
          length_buffer_line_file_settings -
          index_value_start -
          5
        );

        settings_name = malloc(
          sizeof(char) *
          index_value_start +
          1
        );

        settings_value = malloc(
          sizeof(char) *
          length_settings_value +
          1
        );

        settings_name[
          index_value_start
        ] = '\0';
        
        settings_value[
          index_value_start
        ] = '\0';

        clic3_bytes_copy(
          settings_name,
          buffer_line_file_settings,
          index_value_start -
          1
        );

        if (
          clic3_char_arrays_within(
            settings_name,
            1,
            "volume"
          ) == 0
        ) {
          clic3_bytes_copy(
            settings_value,
            buffer_line_file_settings +
            index_value_start +
            2,
            length_settings_value
          );

          clic3_char_array_to_float(
            settings_value,
            &settings->volume
          );
        }

        length_buffer_line_file_settings = 0;

        free(
          settings_name
        );

        free(
          settings_value
        );
      }

      character_file_settings = getc(
        file_settings
      );
    }

    free(
      buffer_line_file_settings
    );
  }

  fclose(
    file_settings
  );
}
