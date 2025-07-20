#ifndef __io_proc_h
#define __io_proc_h

#include <CoreAudio/CoreAudio.h>

OSStatus io_proc(
  AudioObjectID,
  const AudioTimeStamp*,
  const AudioBufferList*,
  const AudioTimeStamp*,
  AudioBufferList*,
  const AudioTimeStamp*,
  void*
);

#endif

