#include "file_handling.h"

// check length of output name and check name of ouput have no " \/:*?\"<>| "
bool is_over_range(char *file_name)
{
    bool valid_input;
    bool check_exist_file = false;
    if (strlen(file_name) > MAX_FILE_NAME)
    {
        printf(">> File name too long. Please enter a shorter file name.\n");
        return check_exist_file;
    }

    valid_input = is_invalid_file_name(file_name);
    if (!valid_input)
    {
        printf(">> Invalid input. Do not enter: \\/:*?\"<>| \n");
        return check_exist_file;
    }
    else
    {
        check_exist_file = is_exist_out_file(file_name);
    }
    return check_exist_file;
}

// enter name of file
void get_file_name(char *file_name)
{
    __fpurge(stdin);
    printf("Enter your output file name: ");
    fgets(file_name, MAX_FILE_NAME, stdin);
    file_name[strcspn(file_name, "\n")] = '\0';
}