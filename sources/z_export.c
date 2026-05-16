#include <z_export.h>

#include <z_io_proc.h>
#include <z_io_proc_data.h>

#include <clic3_memory.h>

#include <wave_chunk_data.h>
#include <wave_chunk_fact.h>
#include <wave_chunk_fmt.h>
#include <wave_chunk_riff.h>
#include <wave_format.h>
#include <wave_parameters.h>

#include <stdio.h>

unsigned int z_export_length_samples_get(
  unsigned short int bytes_sample,
  unsigned int length_channels,
  unsigned int length
) {
  return (
    bytes_sample *
    length_channels *
    length *    0x64
  );
}

unsigned char z_export(
  char* path_file_export,
  struct z_io_proc_data* z_io_proc_data
) {
  struct wave_parameters wave_parameters = {
    .wave_format = (
      z_export_default_wave_format
    ),
    .length_channels = (
      z_export_default_length_channels
    ),
    .rate_samples = (
      z_export_default_rate_samples
    ),
    .bytes_sample = (
      z_export_default_bytes_sample
    ),
    .length_samples = (
      z_export_length_samples_get(
        z_export_default_bytes_sample,
        z_export_default_length_channels,
        z_io_proc_data->queue.track_current->length
      )
    )
  };

  return (
    z_export_with_parameters(
      path_file_export,
      z_io_proc_data,
      &wave_parameters
    )
  );
}

unsigned char z_export_with_parameters(
  char* path_file_export,
  struct z_io_proc_data* z_io_proc_data,
  struct wave_parameters* wave_parameters
) {
  FILE* output = (
    fopen(
      path_file_export,
      "wb"
    )
  );

  if (
    output ==
    0x00
  ) {
    return (
      0x01
    );
  }

  struct wave_chunk_data chunk_data;
  struct wave_chunk_fmt chunk_fmt;
  struct wave_chunk_riff chunk_riff;

  wave_chunk_riff_initialize(
    &chunk_riff,
    wave_parameters
  );

  wave_chunk_fmt_initialize(
    &chunk_fmt,
    wave_parameters
  );

  wave_chunk_data_initialize(
    &chunk_data,
    wave_parameters
  );

  float rate_samples = (
    wave_parameters->rate_samples *
    0x02
  );

  unsigned char* bytes = (
    clic3_memory_allocate_raw(
      wave_parameters->bytes_sample
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
      wave_parameters->bytes_sample
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
      wave_parameters->length_samples *
      wave_parameters->length_channels
    );
    ++frame
  ) {
    unsigned long int index_data = (
      frame *
      wave_parameters->bytes_sample
    );

    unsigned long int value = (
      0x00
    );

    unsigned long int index = (
      frame /
      wave_parameters->length_channels
    );

    if (
      (
        frame %
        wave_parameters->length_channels
      ) ==
      0x00
    ){
      z_io_proc_data->frame = (
        z_io_proc_data->frame +
        0x01
      );
    
      float value_frame = (
        z_io_proc_frame_value_get(
          z_io_proc_data,
          &z_io_proc_data->queue
        )
      );
      value = (
        (
          value_frame +
          0x01
        ) /
        0x02 *
        maximum_bytes
      );
    } else {
      value = (
        (
          value +
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
        wave_parameters->bytes_sample
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

  return (
    0x00
  );
}
