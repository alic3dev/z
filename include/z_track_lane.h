#ifndef __z_track_lane_h
#define __z_track_lane_h

#include <z_track_note.h>

#include <cer0_synthesizer.h>

struct z_track_lane {
  unsigned long int index_note;

  unsigned long int length_notes;
  struct z_track_note* notes;

  struct cer0_synthesizer synthesizer;
  
  unsigned char type;
};

#endif
