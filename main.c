#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// Compile with -pthread
// Windows
//#include <C:\Users\Tejan\Documents\Workspace\C Projects\EventQueueExercise\eventqueue.h>
// Mac
#include </Users/tejanc/Documents/XCode/EventQueue/EventQueue/eventqueue.h>

struct arg_struct {
    event_queue_t *arg1;
    event_t *arg2;
};

int main()
{
    printf("EventQueueExercise_v1.1\n");

    event_queue_t * eq = NULL;
    event_t * e = NULL;
    pthread_t tid1, tid2, tid3;
    event_t tdata;
    struct arg_struct *args = malloc(sizeof(struct arg_struct));
    int producer1, producer2, consumer = 0;

    
    // DEMO 1: Add 10 events Remove 5 events.
    
    printf("\n\nDemo 1: Add 10 Remove 5\n\n");
    
    // Get the event queue structure pointer.
    eq = event_queue_init();
    printf("\n\nSize of this pointer is %lu bytes.\n", sizeof(eq));
    printf("Event queue initialized with structure pointer: %d\n", &eq);

    
    for(int i = 0; i < 10; i++)
    {
        // Create a test event
        e = malloc(sizeof(event_t));
        e->name = "TestEvent";
        e->format = "TestFormat";
        e->data = i;
        e->nbytes = sizeof(e->data);

        printf("Adding event...");
        event_queue_add_event(eq, e);
        displayQueue(eq);
    }
    
    for (int i = 0; i < 5; i++)
    {
        event_t *event;
        printf("Getting event...");
        event = event_queue_get_event(eq);
        displayQueue(eq);
    }

    event_queue_destroy(eq);
    
    printf("\n\nDemo 2: Producer consumer threads.\n\n");
    
    // Get the event queue structure pointer.
    eq = event_queue_init();
    printf("\n\nSize of this pointer is %lu bytes.\n", sizeof(eq));
    printf("Event queue initialized with structure pointer: %d\n", &eq);
    
    for( int i = 0; i < 20; i++) {
        
        // Create a test event
        e = malloc(sizeof(event_t));
        e->name = "TestEvent";
        e->format = "TestFormat";
        e->nbytes = sizeof(e->data);
        e->data = i;
        
        args->arg1 = eq;
        args->arg2 = e;
        
        // Create threads
        producer1 = pthread_create( &tid1, NULL, (void *) event_queue_add_event, (void*) args);
        printf("Running thread at %d, Adding event->data = %d\n", &tid1, e->data);
        displayQueue(args->arg1);
        producer2 = pthread_create( &tid2, NULL, (void *) event_queue_add_event, (void*) args);
        printf("Running thread at %d, Adding event->data = %d\n", &tid2, e->data);
        displayQueue(args->arg1);
        consumer = pthread_create( &tid3, NULL, (void *) event_queue_get_event, (void*) args->arg1);
        printf("Running thread at %d, Getting event from event queue: %d\n", &tid3, tdata.data);
        displayQueue(args->arg1);
        
        //Wait for threads to finish:
        //if (producer1)
            pthread_join( tid1, NULL) ;
        //if (producer2)
            pthread_join( tid2, NULL );
        //if (consumer)
            pthread_join( tid3, (void *) &tdata );
    }
    
    event_queue_destroy(eq);


    return 0;
}
