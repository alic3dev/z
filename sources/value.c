#include <value.h>

#include <time.h>

float value;
unsigned long int seed_addition = 0;

void value_seed() {
  value = (float) ((time((void*)0) + seed_addition++) % 1000000);
}

float value_get() {
  value = value / 2.0f;

  if (value < 100000) {
    value = value * 10.0f;
  }

  return value;
}
