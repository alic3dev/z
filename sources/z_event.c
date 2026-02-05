#include <z_event.h>

#include <clic3_memory.h>

unsigned char z_event_functions_length = 0;

z_event_function* z_event_functions = (
  (void*) 0
);

enum z_event_type* z_event_types = (
  (void*) 0
);

void z_event_initialize() {
  z_event_functions = (
    clic3_memory_allocate_raw(
      sizeof(
        z_event_function
      ) *
      z_event_functions_length
    )
  );

  z_event_types = (
    clic3_memory_allocate_raw(
      sizeof(
        enum z_event_type
      ) *
      z_event_functions_length
    )
  );
}

void z_event_trigger(
  enum z_event_type z_event_type,
  void* z_event_data
) {
  for (
    unsigned char index_z_event_function = 0;
    index_z_event_function < z_event_functions_length;
    ++index_z_event_function
  ) {
    if (
      z_event_types[
        index_z_event_function
      ] == z_event_type
    ) {
      z_event_functions[
        index_z_event_function
      ](
        z_event_data
      );
    }
  }
}

unsigned char z_event_on(
  z_event_function z_event_function,
  enum z_event_type z_event_type
) {
  if (
    z_event_functions_length >= 255
  ) {
    return 1;
  }

  z_event_functions_length = (
    z_event_functions_length +
    1
  );

  clic3_memory_reallocate_raw(
    &z_event_functions,
    (
      sizeof(
        z_event_function
      ) *
      z_event_functions_length
    )
  );

  clic3_memory_reallocate_raw(
    &z_event_types,
    (
      sizeof(
        enum z_event_type
      ) *
      z_event_functions_length
    )
  );

  z_event_functions[
    z_event_functions_length -
    1
  ] = (
    z_event_function
  );

  z_event_types[
    z_event_functions_length
  ] = (
    z_event_type
  );

  return 0;
}

unsigned char z_event_off(
  z_event_function z_event_function,
  enum z_event_type z_event_type
) {
  for (
    unsigned char index_z_event_function = 0;
    index_z_event_function < z_event_functions_length;
    ++index_z_event_function
  ) {
    if (
      z_event_functions[
        index_z_event_function
      ] &&
      z_event_types[
        index_z_event_function
      ]
    ) {
      for (
        unsigned char index_z_event_function_removal = index_z_event_function;
        index_z_event_function_removal < z_event_functions_length - 1;
        ++index_z_event_function_removal
      ) {
        z_event_functions[
          index_z_event_function_removal
        ] = z_event_functions[
          index_z_event_function_removal +
          1
        ];

        z_event_types[
          index_z_event_function_removal
        ] = z_event_types[
          index_z_event_function_removal +
          1
        ];
      }

      z_event_functions_length = (
        z_event_functions_length -
        1
      );

      clic3_memory_reallocate_raw(
        &z_event_functions,
        (
          sizeof(
            z_event_function
          ) *
          z_event_functions_length
        )
      );

      clic3_memory_reallocate_raw(
        &z_event_types,
        (
          sizeof(
            enum z_event_type
          ) *
          z_event_functions_length
        )
      );

      return 1;
    }
  }

  return 0;
}

void z_event_destroy() {
  clic3_memory_free_raw(
    z_event_functions
  );

  clic3_memory_free_raw(
    z_event_types
  );
}
