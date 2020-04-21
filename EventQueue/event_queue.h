//
//  event_queue.h
//  EventQueue
//
//  Created by Tejan C on 2019-10-17.
//  Copyright © 2019 Crank Software Inc. All rights reserved.
//

#ifndef event_queue_h
#define event_queue_h

#include <stdio.h>
#include <stdlib.h>

typedef struct _event_queue event_queue_t;

typedef struct _event_container event_container_t;

typedef struct _event{
    char        *name;           // Name of the event
    char        *format;         // Format string that describes the event
    int32_t     nbytes;          // Number of bytes of data
    void        *data;           // Event data payload (nBytes worth of data)
} event_t;

/**
 * Initialized and create an event queue
 * @return A valid event queue structure pointer, or NULL on failure
 */
event_queue_t *event_queue_init(void);

/**
 * Destroy a previously initialized event queue.
 * After this operation the event queue pointer is no longer valid and all memory
 * allocated by the queue will be released.
 * @param eq  A pointer to a valid event queue
 */
void event_queue_destroy(event_queue_t *eq);

/**
 * Add an event to an event queue
 * After this call returns, the client can de-allocate any memory associated with
 * the event or the event fields (inluding the data) as it has been internally transferred
 * to the event queue.
 * @param eq    The event queue to add an event to
 * @param event The event to add
 * @return 0 on success or a non-zero value on error
 */
int event_queue_add_event(event_queue_t *eq, event_t *event);

 /**
  * Pull an event from the event queue
  * This is a blocking call that will wait for an event to be available in the event queue
  * and will then return that event as a pointer.
  * Clients must call event_queue_event_release() with the returned event pointer
  *
  * @param eq    The event queue to get an event from
  * @return The next event or NULL on error
  */
event_t * event_queue_get_event(event_queue_t *eq);

/**
 * Release the resources associated with the event returned from event_queue_get_event()
 * @param eq The event queue that provided this event
 * @param event An event returned from event_queue_get_event()
 * @return 0 on success or non-zero value on error
 */
int event_queue_event_release(event_queue_t *eq, event_t *event);



void display_event_queue(event_queue_t *eq);
void display_event_properties(event_container_t *event);

#endif /* event_queue_h */
