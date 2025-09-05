#ifndef __track_threads_h
#define __track_threads_h

#include <pthread.h>

extern pthread_mutex_t track_threads_mutex_track_completion;
extern pthread_mutex_t track_threads_mutex_track_generating;
extern pthread_t track_threads_thread_completion_handler;

void track_threads_initialize();
void* track_threads_completion_handler(void*);
void track_threads_destroy();

#endif
