#include <z_event.h>

#include <clic3_memory.h>

void z_event_initialize(
  struct z_event_store* z_event_store
) {
  z_event_store->length = (
    0x00
  );

  z_event_store->structures = (
    clic3_memory_allocate_raw(
      0x00
    )
  );
}

void z_event_trigger(
  struct z_event_store* z_event_store,
  enum z_event_type z_event_type,
  void* z_event_data
) {
  for (
    unsigned char index_z_event_function_structure = (
      0x00
    );
    (
      index_z_event_function_structure <
      z_event_store->length
    );
    ++index_z_event_function_structure
  ) {
    struct z_event_function_structure* z_event_function_structure = (
      z_event_store->structures[
        index_z_event_function_structure
      ]
    );

    if (
      z_event_function_structure->event_type !=
      z_event_type
    ) {
      continue;
    }

    switch (
      z_event_function_structure->type
    ) {
      case z_event_function_type_with_data: {
        z_event_function_with_data z_event_function_structure_function = (
          z_event_function_structure->function
        );

        void* z_event_function_structure_data = (
          z_event_function_structure->data
        );

        z_event_function_structure_function(
          z_event_data,
          z_event_function_structure_data
        );

        break;
      }
      case z_event_function_type_without_data: {
        z_event_function_without_data z_event_function_structure_function = (
          z_event_function_structure->function
        );

        z_event_function_structure_function(
          z_event_data
        );

        break;
      }
      default: {
        break;
      }
    }
  }
}

void z_event_on(
  struct z_event_store* z_event_store,
  z_event_function_without_data z_event_function,
  enum z_event_type z_event_type
) {
  z_event_function_structures_add(
    z_event_store,
    z_event_function,
    z_event_type,
    z_event_function_type_without_data,
    0x00
  );
}

void z_event_on_with_data(
  struct z_event_store* z_event_store,
  z_event_function_with_data z_event_function,
  enum z_event_type z_event_type,
  void* z_event_function_data
) {
  z_event_function_structures_add(
    z_event_store,
    z_event_function,
    z_event_type,
    z_event_function_type_with_data,
    z_event_function_data
  );
}

void z_event_function_structures_add(
  struct z_event_store* z_event_store,
  void* z_event_function_structure_function,
  enum z_event_type z_event_function_structure_event_type,
  enum z_event_function_type z_event_function_structure_type,
  void* z_event_function_structure_data
) {
  unsigned int index_z_event_function_structure = (
    z_event_store->length
  );

  z_event_store->length = (
    z_event_store->length +
    0x01
  );

  clic3_memory_reallocate_raw(
    &z_event_store->structures,
    (
      sizeof(
        struct z_event_function_structure*
      ) *
      z_event_store->length
    )
  );

  z_event_store->structures[
    index_z_event_function_structure
  ] = (
    clic3_memory_allocate_raw(
      sizeof(
        struct z_event_function_structure
      )
    )
  );

  struct z_event_function_structure* z_event_function_structure = (
    z_event_store->structures[
      z_event_store->length -
      0x01
    ]
  );

  z_event_function_structure->function = (
    z_event_function_structure_function
  );

  z_event_function_structure->event_type = (
    z_event_function_structure_event_type
  );

  z_event_function_structure->type = (
    z_event_function_structure_type
  );

  z_event_function_structure->data = (
    z_event_function_structure_data
  );
}

unsigned char z_event_off(
  struct z_event_store* z_event_store,
  void* z_event_function_structure_function,
  enum z_event_type z_event_function_structure_event_type
) {
  for (
    unsigned char index_z_event_function_structure = (
      0x00
    );
    (
      index_z_event_function_structure <
      z_event_store->length
    );
    ++index_z_event_function_structure
  ) {
    struct z_event_function_structure* z_event_function_structure = (
      z_event_store->structures[
        index_z_event_function_structure
      ]
    );

    if (
      (
        z_event_function_structure !=
        z_event_function_structure_function
      ) ||
      (
        z_event_function_structure->event_type !=
        z_event_function_structure_event_type
      )
    ) {
      continue;
    }

    clic3_memory_free_raw(
      z_event_function_structure
    );

    for (
      unsigned char index_z_event_function_structure_removal = (
        index_z_event_function_structure
      );
      (
        index_z_event_function_structure_removal <
        (
          z_event_store->length -
          0x01
        )
      );
      ++index_z_event_function_structure_removal
    ) {
      z_event_store->structures[
        index_z_event_function_structure_removal
      ] = (
        z_event_store->structures[
          index_z_event_function_structure_removal +
          0x01
        ]
      );
    }

    z_event_store->length = (
      z_event_store->length -
      0x01
    );

    clic3_memory_reallocate_raw(
      &z_event_store->structures,
      (
        sizeof(
          struct z_event_function_structure*
        ) *
        z_event_store->length
      )
    );

    return (
      0x00
    );
  }

  return (
    0x01
  );
}

void z_event_destroy(
  struct z_event_store* z_event_store
) {
  for (
    unsigned char index_z_event_function_structure = (
      0x00
    );
    (
      index_z_event_function_structure <
      z_event_store->length
    );
    ++index_z_event_function_structure
  ) {
    struct z_event_function_structure* z_event_function_structure = (
      z_event_store->structures[
        index_z_event_function_structure
      ]
    );

    clic3_memory_free_raw(
      z_event_function_structure
    );
  }

  clic3_memory_free_raw(
    z_event_store->structures
  );
}
