#ifndef __z_event_h
#define __z_event_h

enum z_event_type {
  z_event_type_track_pregeneration = 0x00,
  z_event_type_track_update        = 0xff
};

enum z_event_function_type {
  z_event_function_type_without_data = 0x00,
  z_event_function_type_with_data    = 0xff
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

struct z_event_store {
  struct z_event_function_structure** structures;  unsigned int length;
};

void z_event_initialize(
  struct z_event_store*
);

void z_event_trigger(
  struct z_event_store*,
  enum z_event_type,
  void*
);

void z_event_on(
  struct z_event_store*,
  z_event_function_without_data,
  enum z_event_type
);

void z_event_on_with_data(
  struct z_event_store*,
  z_event_function_with_data,
  enum z_event_type,
  void*
);

void z_event_function_structures_add(
  struct z_event_store*,
  void* function,
  enum z_event_type event_type,
  enum z_event_function_type type,
  void* data
);

unsigned char z_event_off(
  struct z_event_store*,
  void*,
  enum z_event_type
);

void z_event_destroy(
  struct z_event_store*
);

#endif
