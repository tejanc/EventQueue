#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// Compile with -pthread
#include <C:\Users\Tejan\Documents\Workspace\C Projects\EventQueueExercise\eventqueue.h>

int main()
{
    printf("EventQueueExercise_v1.0\n");

    event_queue_t * eq;
    event_t * e;
    pthread_t tid1, tid2, tid3;
    event_t tdata;

    // Get the event queue structure pointer.
    eq = event_queue_init();
    printf("size of this pointer is %d bytes.\n", sizeof(eq));
    printf("event queue initialized with structure pointer: %d\n", &eq);

    for(int i = 0; i < 100; i++)
    {
        // Create a test event
        e = malloc(sizeof(event_t));
        e->name = "TestEvent";
        e->format = "TestFormat";
        e->data = i%10;
        e->nbytes = sizeof(e->data);

        int producer1 = pthread_create(&tid1, NULL, (void*)event_queue_add_event(eq, e), NULL);
        printf("Running thread at %d, Adding event->data = %d\n", &tid1, e->data);
        int producer2 = pthread_create(&tid2, NULL, (void*)event_queue_add_event(eq, e), NULL);
        printf("Running thread at %d, Adding event->data = %d\n", &tid2, e->data);
        int consumer = pthread_create(&tid3, NULL, (void*)event_queue_get_event(eq), NULL);
        //Wait for both threads to finish:
        if (producer1) {
            pthread_join(tid1, NULL);
        }
        if (producer2) {
            pthread_join(tid2, NULL);
        }
        if (consumer) {
            pthread_join(tid3, (void*)&tdata);
        }
        printf("Running thread at %d, Getting event from event queue: %d\n", &tid3, tdata.data);

    }

    printf("Size of event queue is: %d", eq->count);

    displayQueue(eq);

    event_queue_destroy(eq);

    return 0;
}
