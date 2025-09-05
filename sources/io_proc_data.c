#include <io_proc_data.h>

#include <track.h>

#include <cer0.h>

void io_proc_data_initialize(
  struct io_proc_data* io_proc_data
) {
  io_proc_data->frame = 0;
  io_proc_data->initialized = 0;
}
