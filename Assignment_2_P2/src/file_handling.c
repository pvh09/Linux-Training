#include "file_handling.h"

bool is_invalid_file_name(const char *file_name)
{
    const char *invalid_chars = "\\/:*?\"<>| ";
    for (int i = 0; i < strlen(invalid_chars); i++)
    {
        if (strchr(file_name, invalid_chars[i]) != NULL)
        {
            return false;
        }
    }
    return true;
}

// check file output is existed in folder
bool is_exist_out_file(char *file_name)
{
    char ch = {0};
    FILE *fp;
    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf(">> Create a output file success!!\n");
        return true;
    }
    else
    {
        printf("Do you want to append output file (Y) or (N)? \n");
        printf("You choose: ");
        ch = getchar();
        if (ch == 'Y' || ch == 'y')
        {
            printf(">> Append output file success!!\n");
            return true;
        }
    }
    printf(">> This file is existed in folder. Please enter other output file!!\n");
    if (fp != NULL)
    {
        fclose(fp);
    }
    return false;
}

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
