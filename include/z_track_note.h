#ifndef __z_track_note_h
#define __z_track_note_h

#include <cer0_attack_sustain_decay_release.h>

struct z_track_note {
  float amplitude;
  float time;
  float value;

  struct cer0_attack_sustain_decay_release_parameters attack_sustain_decay_release_parameters;
};

#endif
