#ifndef __settings_h
#define __settings_h

struct settings {
  float volume;
};

extern struct settings settings;

void settings_initialize();

#endif
