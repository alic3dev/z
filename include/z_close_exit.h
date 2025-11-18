#ifndef __z_close_exit_h
#define __z_close_exit_h

#include <pthread.h>

extern pthread_mutex_t z_close_exit_mutex;

void z_close_exit_initialize();
void z_close_exit();

#endif
