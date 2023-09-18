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

/**
 * Function to input a file name from the user.
 *
 * @param file_name - A pointer to a character array where the file name will be stored.
 */
void input_file(char *file_name);

/**
 * Function to create a hard link to a target file.
 *
 * @param file_name - The name of the target file.
 */
void create_hard_link_function(char *file_name);

/**
 * Function to create a soft (symbolic) link to a target file.
 *
 * @param file_name - The name of the target file.
 */
void create_soft_link_function(char *file_name);
#endif