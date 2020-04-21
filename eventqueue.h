#ifndef EVENTQUEUE_H_INCLUDED
#define EVENTQUEUE_H_INCLUDED

#include <stdint.h>

typedef struct {
  char 		*name;		//Name of the event
  char 		*format;	//Format string that somehow describes the event
  int32_t 	nbytes;     //Number of bytes of data
  void      *data;		//Event data payload (nbytes worth of data)
  struct event_t   *next;
} event_t;

typedef struct {
  char      *name;      // Name of the event queue
  event_t   *head;      // Holds a pointer to the head event.
  event_t   *tail;      // Holds a pointer to the tail event.
} event_queue_t;

void displayQueue(event_queue_t *eq);
event_queue_t *event_queue_init(void);
void event_queue_destroy(event_queue_t *eq);
int event_queue_add_event(event_queue_t *eq, event_t *event);
event_t *event_queue_get_event(event_queue_t *eq);
int event_queue_event_release(event_queue_t *eq, event_t *event);

#endif // EVENTQUEUE_H_INCLUDED
