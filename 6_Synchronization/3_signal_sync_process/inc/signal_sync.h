#ifndef _SIGNAL_PROCESS_H_
#define _SIGNAL_PROCESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define MAX_SIZE 256
#define OUTPUT_FILE "output.txt"

void parent_process();

#endif
