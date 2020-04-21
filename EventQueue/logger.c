//
//  logger.c
//  EventQueue
//
//  Created by Tejan C on 2019-10-17.
//  Copyright © 2019 Crank Software Inc. All rights reserved.
//

#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void logger(const char* tag, const char* message) {
   time_t now;
   time(&now);
   printf("%s [%s]: %s\n", ctime(&now), tag, message);
}


