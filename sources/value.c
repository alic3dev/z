#include <value.h>

#include <stdlib.h>
#include <time.h>

float value_seed_value = 0.0f;
float value_value = 0.0f;

unsigned long int value_seed_addition = 0;

void value_seed() {
  value_seed_value = (
    (float) ((
      time((void*)0) + value_seed_addition++
    ) % 1000000)
  );
  value_value = value_seed_value;
}

void value_buffer_seed_set(
  unsigned char* buffer
) {
  for (
    unsigned char index_buffer = 0;
    index_buffer < sizeof(float);
    ++index_buffer
  ) {
    buffer[
      index_buffer
    ] = (
      ((unsigned char*) &value_seed_value)[
        index_buffer
      ]
    );
  }
}

float value_get() {
  value_value = value_value / 2.0f;

  if (value_value < 100000) {
    value_value = value_value * 10.0f;
  }

  return value_value;
}
