#ifndef _SYNC_FILE_H_
#define _SYNC_FILE_H_

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h> 

#define MAX_FILE_NAME 20

void init_semaphore();

#endif
