#ifndef _FILE_HANDLING_H_
#define _FILE_HANDLING_H_
#include "directory.h"

/**
 * @brief Check if a string consists of digits.
 *
 * This function checks if a given string consists of digits.
 *
 * @param file_name The string to be checked.
 * 
 * @return True if the string consists of digits; otherwise, false.
 */
bool check_digit(char* file_name);

/**
 * @brief Check if a file input exists in the folder.
 *
 * This function checks if a file input exists in the folder.
 *
 * @param file_name The name of the input file to be checked.
 * 
 * @return True if the file exists; otherwise, false.
 */
bool is_exist_input_file(char* file_name);

/**
 * @brief Check if a file exists in the folder.
 *
 * This function checks if a file exists in the folder based on the specified file name.
 *
 * @param file_name The name of the file to be checked.
 * 
 * @return True if the file exists; otherwise, false.
 */
bool check_file_exist(char* file_name);

/**
 * @brief Check if a file name is invalid.
 *
 * This function checks if a file name contains invalid characters.
 *
 * @param file_name The file name to be checked.
 * 
 * @return True if the file name is invalid; otherwise, false.
 */

void get_file_name(char* file_name, bool flag_check_input_output);

/**
 * @brief Input data from a file.
 *
 * This function reads data from a file specified by the file name.
 *
 * @param file_name The name of the input file.
 */


#endif