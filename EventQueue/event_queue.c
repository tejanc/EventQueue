//
//  event_queue.c
//  EventQueue
//
//  Created by Tejan C on 2019-10-17.
//  Copyright © 2019 Crank Software Inc. All rights reserved.
//

#include "event_queue.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_EQ_EVENT_SIZE 65535


typedef struct _event_queue{
    pthread_mutex_t     mutex;
    pthread_cond_t      cond;
    event_container_t   *first_event;
    event_container_t   *last_event;
} event_queue_t;


typedef struct _event_container {
    event_t             event;
    event_container_t   *next_event;  // An address to the next event to be de-qued.
    event_container_t   *previous_event;  // An address to the previous event of this event.
} event_container_t;


event_queue_t *event_queue_init() {
    int             ret;
    event_queue_t   *eq;
    
    eq = calloc(1, sizeof(event_queue_t));
    if (eq == NULL) {
        logger("FATAL", "Out of memory..");
        return NULL;
    }
    
    ret = pthread_mutex_init(&eq->mutex, NULL);
    if (ret != 0) {
        event_queue_destroy(eq);
        return NULL;
    }
    
    ret = pthread_cond_init(&eq->cond, NULL);
    if (ret != 0) {
        pthread_mutex_destroy(&eq->mutex);
        event_queue_destroy(eq);
        return NULL;
    }
    
    return eq;

}


void event_queue_destroy(event_queue_t *eq) {
    pthread_cond_destroy(&eq->cond);
    pthread_mutex_destroy(&eq->mutex);
    free(eq);
}


int event_queue_add_event(event_queue_t *eq, event_t *event) {
    
    event_container_t   *event_container;
    event_t             *event_copy;
    
    unsigned long   total_nbytes;
    unsigned long   name_nbytes;
    unsigned long   fmt_nbytes;
    
    int     data_nbytes;
    int     container_nbytes;
    
    void    *copy_ptr;
    
    //Protect event queue against events that are larger than 64K
    if (event->nbytes > MAX_EQ_EVENT_SIZE) {
        logger("WARNING", "Dropping event. The event data size is greater than the maximum allowed.");
        return -1;
    }
    
    //Calculate sizes
    container_nbytes = sizeof(event_container_t);
    name_nbytes = strlen(event->name) + 1;
    fmt_nbytes = strlen(event->format) + 1;
    data_nbytes = event->nbytes;
    
    total_nbytes = container_nbytes + name_nbytes + fmt_nbytes + data_nbytes;
    
    // Event Container
    // ___________________
    //| *name             |
    //| *format           |
    //| nbytes            |
    //| *data             |
    //|-------------------|
    //| name + \0         |
    //|-------------------|
    //| format + \0       |
    //|-------------------|
    //| data payload      |
    // -------------------
    //Prepare and fill the event container.
    
    event_container = calloc(1, total_nbytes);
    if (event_container == NULL) {
        logger("FATAL", "Out of memory...");
        return -1;
    }
    
    copy_ptr = event_container;
    memcpy(copy_ptr, event, sizeof(event_t));
    event_copy = (event_t *) copy_ptr;
    copy_ptr += container_nbytes;
    memcpy(copy_ptr, event_copy->name, name_nbytes);
    event->name = copy_ptr;
    copy_ptr += name_nbytes;
    memcpy(copy_ptr, event_copy->format, fmt_nbytes);
    event->format = copy_ptr;
    copy_ptr += fmt_nbytes;
    memcpy(copy_ptr, event_copy->data, data_nbytes);
    event->data = copy_ptr;
    
    /* CRITICAL SECTION */
    
    pthread_mutex_lock(&eq->mutex);
    
    //If the last event is already nothing, then it means that the first event is also nothing (i.e. it got free'd).
    if (eq->last_event == NULL) {
        eq->first_event = NULL;
    }
    
    //Add the event to the top of this event queue.
    if (eq->first_event == NULL) {
        eq->first_event = eq->last_event = event_container;
    }
    else {
        event_container->next_event = eq->first_event;
        eq->first_event->previous_event = event_container;
        //update the new head to become the copied event container (contains the latest event).
        eq->first_event = event_container;
    }
    
    pthread_cond_broadcast(&eq->cond);
    pthread_mutex_unlock(&eq->mutex);
    
    /* EXIT CRITICAL SECTION */
    
    return 0;
}

event_t * event_queue_get_event(event_queue_t *eq) {
    
    event_t     *ret_event;
    struct timespec    timeout;
    
    clock_gettime(CLOCK_MONOTONIC, &timeout);
    timeout.tv_sec += 5;
    
    pthread_mutex_lock(&eq->mutex);
    
    while (eq->first_event == NULL) {
        pthread_cond_timedwait(&eq->cond, &eq->mutex, &timeout);
    }

    //Guard condition
    //if (eq->first_event == NULL) {
    //    logger("WARNING", "There are no events to return at this moment.");
    //    pthread_mutex_unlock(&eq->mutex);
    //    return NULL;
    //}
    
    /* There is something to return */
    ret_event = &eq->last_event->event;
    
    //Handle more than one get_event by updating the last_event (earliest event).
    if (eq->last_event != NULL) {
        eq->last_event = eq->last_event->previous_event;
    }
    else {
        eq->first_event = NULL;
    }
    
    pthread_mutex_unlock(&eq->mutex);
    
    return ret_event;
    
}


int event_queue_event_release(event_queue_t *eq, event_t *event) {
    
    //Assumption: The user is sending the correct event associated with the correct event queue.
    event_container_t *container_to_free;
    
    //Set preconditions fully before freeing memory.
    
    //When one event is in the event queue.
    container_to_free = (event_container_t *) event;
    
    if (container_to_free != NULL) {
        free(container_to_free);
    } else {
        /* Nothing to free */
        logger("WARNING", "There was nothing to free!");
    }
    
    return 0;
}


void display_event_properties(event_container_t *ec) {

    int * a;
    
    a = ec->event.data;
    
    printf("name:   [%s]\n", ec->event.name);
    printf("format: [%s]\n", ec->event.format);
    printf("nBytes: [%d]\n", ec->event.nbytes);
    printf("data:  ");
    for (int i = 0; i < ec->event.nbytes / 4; i++) {
        printf(" [%d],", a[i]);
    }
    printf("\n");
    
}


void display_event_queue(event_queue_t *eq) {

    event_container_t *current_event;
    int i = 0;
    
    current_event = eq->last_event;
    
    while(current_event != NULL) {
        printf("Event Item : [%d]\n", i);
        display_event_properties(current_event);
        current_event = current_event->previous_event;
        ++i;
    }
        
        
}
               

