#ifndef __z_event_h
#define __z_event_h

enum z_event_type {
  z_event_type_track_update = 0,
  z_event_type_track_pregeneration = 1
};

typedef void (*z_event_function)(
  void*
);

extern unsigned char z_event_functions_length;
extern z_event_function* z_event_functions;
extern enum z_event_type* z_event_types;

void z_event_initialize();

void z_event_trigger(
  enum z_event_type,
  void*
);

unsigned char z_event_on(
  z_event_function,
  enum z_event_type
);

unsigned char z_event_off(
  z_event_function,
  enum z_event_type
);

void z_event_destroy();

#endif
