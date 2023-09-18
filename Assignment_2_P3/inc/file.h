#ifndef _FILE_H_
#define _FILE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include "file_handling.h"

#define MAX_FILE_NAME 100

void input_file(char *file_name);

void create_hard_link_function(char *file_name);

void create_soft_link_function(char *file_name);
#endif