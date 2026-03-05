#ifndef __z_io_proc_h
#define __z_io_proc_h

#include <z_io_proc_data.h>
#include <z_queue.h>

#if target_os_ios
#else
#include <CoreAudio/CoreAudio.h>
#endif

#if target_os_ios
#else
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

void z_io_proc_frame_get(
  struct z_io_proc_data*,
  struct z_queue*,
  float*,
  unsigned long int,
  unsigned long int
);

#endif
