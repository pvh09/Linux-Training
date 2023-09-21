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

/**
 * @brief Output data to a file.
 *
 * This function outputs data to a file specified by the output file name.
 *
 * @param output_file_name The name of the output file.
 */
void create_output_file(char *output_file_name);

/**
 * @brief Output data to a file.
 *
 * This function enter data from keyboard to get data into output file name.
 *
 * @param output_file_name The name of the output file.
 */
void enter_and_read_from_keyboard(char *output_file);
#endif