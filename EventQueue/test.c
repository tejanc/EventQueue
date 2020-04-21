//
//  test.c
//  EventQueue
//
//  Created by Tejan C on 2019-10-17.
//  Copyright © 2019 Crank Software Inc. All rights reserved.
//

#include "test.h"
#include "event_queue.h"
#include "logger.h"
#include "string.h"
#include <stdlib.h>

void * test_event_init(event_t *ev, const char *event_name, const char *format, int32_t nBytes, void* data) {
    
    ev->name = event_name;
    ev->format = format;
    ev->nbytes = nBytes;
    ev->data = malloc(nBytes);
    if(ev->data == NULL) {
        logger("FATAL", "Out of memory...");
        return NULL;
    }
    memcpy(ev->data, data, nBytes);
    
    return ev;
}

event_t * get_test_event(event_queue_t *eq) {
    
    event_t * ret_event;
    
    printf("\nGetting event from event queue.\n");
    
    ret_event = event_queue_get_event(eq);
    if (ret_event == NULL) {
        logger("FATAL", "Error while returning event.");
        return NULL;
    }
    display_test_event(ret_event);
    //event_queue_event_release(eq, ret_event);
    display_event_queue(eq);
    
    return ret_event;
}

void add_test_event(event_queue_t *eq, event_t * test_event) {
    int ret = 0;
    
    printf("Adding event\n");
    ret = event_queue_add_event(eq, test_event);
    if (ret == 0) {
        display_event_queue(eq);
    } else {
        logger("FATAL","Error in event_queue_add_event");
    }
    printf("\n");
}

void display_test_event_data(int * data, int nbytes) {
    for (int i = 0; i < (nbytes / 4); i++) {
        printf(" [%d],", data[i]);
    }
}

void display_test_event(event_t * test_event) {
    
    printf("name:   [%s]\n", test_event->name);
    printf("format: [%s]\n", test_event->format);
    printf("nBytes: [%d]\n", test_event->nbytes);
    printf("data:  ");
    display_test_event_data(test_event->data, test_event->nbytes);
    printf("\n\n");
    
}





