#ifndef _FILE_HANDLING_H_
#define _FILE_HANDLING_H_
#include "directory.h"

/**
 * @brief Check if a file name is invalid.
 *
 * This function checks if a file name contains invalid characters.
 *
 * @param file_name The file name to be checked.
 *
 * @return True if the file name is invalid; otherwise, false.
 */
bool is_invalid_file_name(const char *file_name);

/**
 * @brief Check if a file output exists in the folder.
 *
 * This function checks if a file output exists in the folder.
 *
 * @param file_name The name of the output file to be checked.
 *
 * @return True if the file exists; otherwise, false.
 */
bool is_exist_out_file(char *file_name);

/**
 * @brief Check if a file name exceeds a specified length or contains invalid characters.
 *
 * This function checks if a file name exceeds a specified length or contains invalid characters such as " \/:*?\"<>|".
 *
 * @param file_name The file name to be checked.
 *
 * @return True if the file name exceeds the length or contains invalid characters; otherwise, false.
 */
bool is_over_range(char *file_name);

/**
 * @brief Get the name of a file.
 *
 * This function allows the user to enter the name of a file.
 *
 * @param file_name The variable to store the entered file name.
 *
 * @param flag_check_input_output A flag to indicate whether the file name is for input or output.
 */
void get_file_name(char *file_name);

#endif