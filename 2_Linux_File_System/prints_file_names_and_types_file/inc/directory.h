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
 * @brief Function to input a file name from the user.
 * 
 * This function prompts the user to enter a file name and stores it in the given
 * character array `file_name`.
 * 
 * @param file_name - A pointer to a character array where the file name will be stored.
 */
void input_file(char *file_name);

/**
 * @brief Function to get a list of sub-files and directories within a specified directory.
 * 
 * Given a directory path specified by `file_name`, this function generates a list of
 * sub-files and directories contained within that directory.
 * 
 * @param file_name - The path of the directory for which to list sub-files and directories.
 */
void get_sub_file(const char *file_name);


#endif