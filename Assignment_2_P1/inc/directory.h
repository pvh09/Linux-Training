#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <ctype.h>
#include <stdbool.h>
#include "file_handling.h"


#define MAX_FILE_NAME 100

void input_file(char *file_name);

void get_sub_file(const char* file_name);

#endif