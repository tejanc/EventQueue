// Windows
//#include <C:\Users\Tejan\Documents\Workspace\C Projects\EventQueueExercise\eventqueue.h>
// Mac
#include </Users/tejanc/Documents/XCode/EventQueue/EventQueue/eventqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Compile with -pthread
// Create a mutex that is ready to be locked.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

event_queue_t * queue;

int isEmpty(event_queue_t *eq) {
    return (eq->head == NULL);
}

void displayQueue(event_queue_t *eq) {

    event_t *temp = eq->head;
    printf("The List is: ");
    while (temp != NULL) {
        printf(" %d",temp->data);
        temp = temp->next;
    }
    printf("\n");
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
    free(eq);
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

    event_t *tmp, *head;
    int eventSize, queueSize;

    tmp = (event_t *)malloc(sizeof(event_t));
    tmp = event;
    tmp->next = NULL;

    head = eq->head;

    // ENTER CRITICAL SECTION
    pthread_mutex_lock(&mutex);

    if (!isEmpty(eq)) {
        
        while(head->next != NULL) {
            head = head->next;
        }
        
        // add the event to end of the event queue.
        head->next = tmp;
        eq->tail = tmp;
        
    }
    else {
        eq->head = eq->tail = tmp;
    }
    

    // EXIT CRITICAL SECTION
    pthread_mutex_unlock(&mutex);

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

    // ENTER CRITICAL SECTION
    pthread_mutex_lock(&mutex);

    event_t *tmp;
    tmp = (event_t *) malloc(sizeof(event_t));
    tmp = queue->head;

    retVal = event_queue_event_release(eq, eq->head);

    if (retVal != 0) {
        printf("Error while releasing event.\n");
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

    if (eq->head != NULL) {
        eq->head = eq->head->next;
        free(event);
        free(eq->head);
    } else {
        return -1;
    }

    return 0;
}


