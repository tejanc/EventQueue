//
//  test.h
//  EventQueue
//
//  Created by Tejan C on 2019-10-17.
//  Copyright © 2019 Crank Software Inc. All rights reserved.
//

#ifndef test_h
#define test_h

#include "event_queue.h"
#include <stdio.h>

#define NUM_ELEMENTS 10

void *test_event_init(event_t *ev, const char *event_name, const char *format, int32_t nBytes, void* data);
void display_test_event(event_t * test_event);
void add_test_event(event_queue_t *eq, event_t * test_event);
event_t * get_test_event(event_queue_t *eq);

#endif /* test_h */
