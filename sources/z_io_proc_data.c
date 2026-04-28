#include <z_io_proc_data.h>

#include <z_settings.h>

void z_io_proc_data_initialize(
  struct z_io_proc_data* z_io_proc_data,
  struct z_track_parameters* z_track_parameters,
  float* rate_sample
) {
  z_io_proc_data->exiting = (
    0x00
  );

  z_io_proc_data->frame = (
    0x00
  );
  
  z_io_proc_data->initialized = (
    0x00
  );

  z_io_proc_data->rate_sample = (
    rate_sample
  );

  z_io_proc_data->track_parameters = (
    z_track_parameters
  );

  z_settings_initialize(
    &z_io_proc_data->settings
  );

  pthread_mutex_init(
    &z_io_proc_data->mutex_exited,
    0x00
  );

  pthread_mutex_init(
    &z_io_proc_data->mutex_playing,
    0x00
  );

  pthread_mutex_lock(
    &z_io_proc_data->mutex_exited
  );
}
