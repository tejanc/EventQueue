#include <C:\Users\Tejan\Documents\Workspace\C Projects\EventQueueExercise\eventqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Compile with -pthread
// Create a mutex that is ready to be locked.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static event_queue_t * queue;

int isEmpty(event_queue_t *eq) {
    return (eq->tail == NULL);
}

void displayQueue(event_queue_t *eq) {

    queue = eq;

    while (eq->count > 0) {
        printf("%s\n",eq->head->data);
        eq->count--;
    }
}

/**
 * Initialized and create an event queue
 * @return A valid event queue structure pointer, or NULL on failure
 */
event_queue_t * event_queue_init() {

    queue = malloc(sizeof(event_queue_t));

    // Check if the queue has already been initialized.
    if (queue->head == NULL) {
        return NULL;
    }

    // Initialize the queue
    queue->name = "myEventQueue";
    queue->count = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;

}

/**
 * Destroy a previously initialized event queue.
 * After this operation the event queue pointer is no longer valid and all memory
 * allocated by the queue will be released.
 * @param eq  A pointer to a valid event queue
 */
void event_queue_destroy(event_queue_t *eq) {
    queue = eq;
    free(queue);
}

/**
 * Add an event to an event queue
 * After this call returns, the client can de-allocate any memory associated with
 * the event or the event fields (inluding the data) as it has been internally transferred
 * to the event queue.
 * @param eq    The event queue to add an event to
 * @param event The event to add
 * @return 0 on success or a non-zero value on error
 */
int event_queue_add_event(event_queue_t *eq, event_t *event) {

    event_t *tmp;
    int eventSize, queueSize;

    tmp = malloc(sizeof(event_t));
    tmp->data = event->data;
    tmp->name = event->name;
    tmp->nbytes = event->nbytes;
    tmp->format = event->format;
    tmp->next = NULL;

    queue = eq;

    eventSize = sizeof(event);
    queueSize = sizeof(queue);

    // ENTER CRITICAL SECTION
    pthread_mutex_lock(&mutex);

    printf("0\n");

    if (!isEmpty(queue)) {

        // dynamically allocate memory for the next event.
        queue = realloc(queue, eventSize + queueSize);

        printf("1\n");

        // add the event to the event queue.
        queue->tail->next = event;
        queue->tail = event;

        printf("2\n");
    }
    else {
        queue = realloc(queue, eventSize + queueSize);
        printf("3\n");
        queue->head = queue->tail = tmp;
        printf("4\n");
    }

    queue->count++;

    // EXIT CRITICAL SECTION
    pthread_mutex_unlock(&mutex);

    printf("5\n");

    return 0;

}

/**
  * Pull an event from the event queue
  * This is a blocking call that will wait for an event to be available in the event queue
  * and will then return that event as a pointer.
  * Clients must call event_queue_event_release() with the returned event pointer
  *
  * @param eq    The event queue to get an event from
  * @return The next event or NULL on error
  */
event_t * event_queue_get_event(event_queue_t *eq) {

    int retVal;
    queue = eq;

    // ENTER CRITICAL SECTION
    pthread_mutex_lock(&mutex);

    event_t *tmp;
    tmp = queue->head;

    retVal = event_queue_event_release(queue, tmp);

    if (retVal != 0) {
        return NULL;
    }

    // EXIT CRITICAL SECTION
    pthread_mutex_unlock(&mutex);

    //pthread_exit(&tmp);

    return(tmp);
}

/**
 * Release the resources associated with the event returned from event_queue_get_event()
 * @param eq The event queue that provided this event
 * @param event An event returned from event_queue_get_event()
 * @return 0 on successs or non-zero value on error
 */
int event_queue_event_release(event_queue_t *eq, event_t *event) {

    queue = eq;

    queue->head = queue->head->next;
    queue->count--;

    free(event);

    return 0;
}


