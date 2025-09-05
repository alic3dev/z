#ifndef __queue_h
#define __queue_h

#include <track.h>

#include <pthread.h>

extern struct track* track_current;
extern struct track* track_upcoming;

extern pthread_mutex_t queue_mutex_track_completion;
extern pthread_mutex_t queue_mutex_track_generating;
extern pthread_t queue_thread_completion_handler;

void queue_initialize();
void* queue_track_completion_handler(void*);
void queue_destroy();

#endif
