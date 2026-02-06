#ifndef __z_event_h
#define __z_event_h

enum z_event_type {
  z_event_type_track_pregeneration = 0x00,
  z_event_type_track_update = 0xff
};

enum z_event_function_type {
  z_event_function_type_without_data = 0x00,
  z_event_function_type_with_data = 0xff
};

typedef void (*z_event_function_without_data)(
  void*
);

typedef void (*z_event_function_with_data)(
  void*,
  void*
);

struct z_event_function_structure {
  void* function;
  enum z_event_type event_type;
  enum z_event_function_type type;
  void* data;
};

extern struct z_event_function_structure** z_event_function_structures;
extern unsigned int z_event_function_structures_length;

void z_event_initialize();

void z_event_trigger(
  enum z_event_type,
  void*
);

void z_event_on(
  z_event_function_without_data,
  enum z_event_type
);

void z_event_on_with_data(
  z_event_function_with_data,
  enum z_event_type,
  void*
);

void z_event_function_structures_add(
  void* function,
  enum z_event_type event_type,
  enum z_event_function_type type,
  void* data
);

unsigned char z_event_off(
  void*,
  enum z_event_type
);

void z_event_destroy();

#endif
