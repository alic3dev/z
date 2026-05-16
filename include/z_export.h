#ifndef __z_export_h
#define __z_export_h

#include <z_track.h>

#include <wave_format.h>
#include <wave_parameters.h>

#define z_export_default_wave_format wave_format_microsoft_pcm_format
#define z_export_default_length_channels 0x02
#define z_export_default_bytes_sample 0x04
#define z_export_default_rate_samples 0xac44

unsigned int z_export_length_samples_get(
  unsigned short int,
  unsigned int,
  unsigned int
);

unsigned char z_export(
  char*,
  struct z_track*
);

unsigned char z_export_with_parameters(
  char*,
  struct z_track*,
  struct wave_parameters*
);

#endif
