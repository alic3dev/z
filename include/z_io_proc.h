#ifndef __z_io_proc_h
#define __z_io_proc_h

#if !target_ios

#include <CoreAudio/CoreAudio.h>

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

#endif
