#ifndef __close_exit_h
#define __close_exit_h

#include <pthread.h>

extern pthread_mutex_t close_exit_mutex;

void close_exit_initialize();
void close_exit();

#endif
