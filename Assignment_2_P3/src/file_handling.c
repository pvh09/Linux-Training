#include "file_handling.h"

// check if enter "/////////" => false
bool check_digit(char *file_name)
{
    int i = 0;
    for (i; i < strlen(file_name); i++)
    {
        if (file_name[i] != '\\' && file_name[i] != '/' && file_name[i] != '.')
        {
            return true;
        }
    }
    return false;
}

// check file input is existed in folder
bool is_exist_input_file(char *file_name)
{
    FILE *fp;
    fp = fopen(file_name, "r");
    if (fp == NULL || !check_digit(file_name))
    {
        printf(">> WARNING: File does not exsit. Please check folder!!\n");
        return false;
    }
    printf(">> Open file successful!!\n");
    fclose(fp);
    return true;
}

// check file exist in folder
bool check_file_exist(char *file_name)
{
    bool check;
    check = is_exist_input_file(file_name);
    return check;
}

// enter name of file
void get_file_name(char *file_name)
{
    __fpurge(stdin);
    printf("Enter your input file name: ");
    fgets(file_name, MAX_FILE_NAME, stdin);
    file_name[strcspn(file_name, "\n")] = '\0';
}
