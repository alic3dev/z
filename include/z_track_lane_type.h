#ifndef __z_z_track_lane_type_h
#define __z_z_track_lane_type_h

#define z_track_lane_types_length 0x05

enum z_track_lane_type {
  z_track_lane_type_bass          = 0x00,
  z_track_lane_type_rhythm_chords = 0x01,
  z_track_lane_type_rhythm_notes  = 0x02,
  z_track_lane_type_chords        = 0x03,
  z_track_lane_type_notes         = 0x04
};

#endif
