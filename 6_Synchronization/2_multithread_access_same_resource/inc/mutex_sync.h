#ifndef _MUTEX_FILE_H_
#define _MUTEX_FILE_H_

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h> 

#define MAX_FILE 100
#define MAX_THREADS 3
#define MAX_SIZE 50

void init_mutex();

#endif
