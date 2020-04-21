//
//  main.c
//  EventQueue
//
//  Created by Tejan C on 2019-10-17.
//  Copyright © 2019 Crank Software Inc. All rights reserved.
//

#include "event_queue.h"
#include "test.h"
#include "logger.h"
#include <stdio.h>
#include <pthread.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Event Queue!\n\n");
    
    // Setup
    event_t         *test_event = NULL;
    event_t         *test_event_two = NULL;
    event_t         *test_event_three = NULL;
    event_t         *test_event_four = NULL;
    event_t         *ret_event = NULL;
    event_t         *ret_event_two = NULL;
    event_t         *ret_event_three = NULL;
    event_t         *ret_event_four = NULL;
    event_t         *ret_event_five = NULL;
    
    event_queue_t   *eq;
    
    const char      *event_name;
    const char      *event_format;
    
    int             event_data[10]      = {10,20,30,40,50,60,70,80,90,100};
    int             event_data_two[23]  = {2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46};
    int             event_data_three[6] = {3,6,9,12,15,18};
    int             event_data_four[4]  = {9,18,27,36};
    
    int32_t         nBytes;
    
    //Initialize and setup
    
    eq = event_queue_init();
    
    test_event = calloc(1, sizeof(event_t));
    if (test_event == NULL) {
        logger("FATAL", "Out of memory...");
        return -1;
    }
    event_name = "Test Event";
    event_format = "Custom Event";
    nBytes = sizeof(event_data);
    
    test_event = test_event_init(test_event, event_name, event_format, nBytes, event_data);
    
    test_event_two = calloc(1, sizeof(event_t));
    if (test_event_two == NULL) {
        logger("FATAL", "Out of memory...");
        return -1;
    }
    event_name = "Test Event";
    event_format = "Custom Event";
    nBytes = sizeof(event_data_two);
    
    test_event_two = test_event_init(test_event_two, event_name, event_format, nBytes, event_data_two);
    
    test_event_three = calloc(1, sizeof(event_t));
    if (test_event_three == NULL) {
        logger("FATAL", "Out of memory...");
        return -1;
    }
    event_name = "Test Event";
    event_format = "Custom Event";
    nBytes = sizeof(event_data_three);
    
    test_event_three = test_event_init(test_event_three, event_name, event_format, nBytes, event_data_three);
    
    test_event_four = calloc(1, sizeof(event_t));
    if (test_event_four == NULL) {
        logger("FATAL", "Out of memory...");
        return -1;
    }
    event_name = "Test Event";
    event_format = "Custom Event";
    nBytes = sizeof(event_data_four);
    
    test_event_four = test_event_init(test_event_four, event_name, event_format, nBytes, event_data_four);

    /* Run Tests */
    
    add_test_event(eq, test_event);
    ret_event = get_test_event(eq);
    event_queue_event_release(eq, ret_event);

    add_test_event(eq, test_event_two);
    add_test_event(eq, test_event_three);
    add_test_event(eq, test_event_four);

    ret_event_two = get_test_event(eq);
    event_queue_event_release(eq, ret_event_two);

    ret_event_three = get_test_event(eq);
    event_queue_event_release(eq, ret_event_three);

    ret_event_four = get_test_event(eq);
    event_queue_event_release(eq, ret_event_four);

    ret_event_five = get_test_event(eq);

    event_queue_event_release(eq, ret_event_five);

//    struct args {
//        event_queue_t   *eq;
//        event_t         *event;
//    };
//
//    struct args *thread_args = malloc (sizeof(struct args));
//    thread_args->eq = eq;
//    thread_args->event = test_event;
//
//    pthread_t thread;
//    if (pthread_create(&thread, NULL, (void *) &add_test_event, (void *) thread_args ) != 0) {
//        perror("pthread_create() error");
//    }
//    pthread_join(thread, NULL);
    
    
    // Teardown
    free(test_event);
    free(test_event_two);
    free(test_event_three);
    free(test_event_four);
    event_queue_destroy(eq);
    
    return 0;
}
