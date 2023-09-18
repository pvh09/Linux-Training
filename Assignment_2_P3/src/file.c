#include "file_handling.h"

// enter input file and check name is validate
void input_file(char *file_name)
{
    bool is_file_exist = false;
    do
    {
        get_file_name(file_name);
        is_file_exist = check_file_exist(file_name);
    } while (!is_file_exist || strlen(file_name) > MAX_FILE_NAME);
}

void create_hard_link_function(char *file_name)
{
    const char *hard_link_file = "hard_link.txt";

    if (link(file_name, hard_link_file) == 0)
    {
        printf("Hard link created successfully.\n");
    }
    else
    {
        perror("Error creating hard link");
    }
}

void create_soft_link_function(char *file_name)
{
    const char *soft_link_file = "soft_link.txt";

    if (symlink(file_name, soft_link_file) == 0)
    {
        printf("Hard link created successfully.\n");
    }
    else
    {
        perror("Error creating hard link");
    }
}
