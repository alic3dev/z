#ifndef __value_h
#define __value_h

extern float value_seed_value;
extern float value_value;
extern unsigned long int value_seed_addition;

void value_seed();

void value_buffer_seed_set(
  unsigned char*
);

float value_get();

#endif
