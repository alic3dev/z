#ifndef __z_io_proc_h
#define __z_io_proc_h

#include <z_io_proc_data.h>
#include <z_queue.h>
#include <z_track.h>

#if !target_os_ios
#include <CoreAudio/CoreAudio.h>
#endif

void* z_io_proc_initializer(
  void*
);

#if !target_os_ios
int z_io_proc(
  AudioObjectID,
  const AudioTimeStamp*,
  const AudioBufferList*,
  const AudioTimeStamp*,
  AudioBufferList*,
  const AudioTimeStamp*,
  void*
);
#endif

void z_io_proc_frame_value_get(
  struct z_track*,
  unsigned long long int,
  float,
  float,
  float[
    0x02
  ],
  unsigned char
);

float z_io_proc_frame_volume_apply(
  float,
  float
);

void z_io_proc_frame_get(
  struct z_io_proc_data*,
  struct z_queue*,
  float*,
  unsigned long int,
  unsigned long int,
  unsigned char
);

#endif
