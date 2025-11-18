#ifndef __z_io_proc_h
#define __z_io_proc_h

#include <CoreAudio/CoreAudio.h>

OSStatus z_io_proc(
  AudioObjectID,
  const AudioTimeStamp*,
  const AudioBufferList*,
  const AudioTimeStamp*,
  AudioBufferList*,
  const AudioTimeStamp*,
  void*
);

#endif
