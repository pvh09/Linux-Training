#include "file_handling.h"

const char *check_file_type(char file_type)
{
    switch (file_type)
    {
    case '-':
        return "Regular file";
    case 'd':
        return "Directory file";
    case 'l':
        return "Link file";
    case 'b':
        return "Block special file";
    case 'p':
        return "Named pipe file";
    case 'c':
        return "Character special file";
    case 's':
        return "Socket file";
    default:
        return "Unknown file type";
    }
}

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

// Function to format and display data from the directory listing
void format_data_from_dir(const char *file_name)
{
    FILE *file = fopen(file_name, "r");

    if (file == NULL)
    {
        perror("Error opening the file");
        return;
    }

    char line[MAX_FILE_NAME];
    int line_count = 0;

    // Print table header
    printf("\n   %-25s%-20s\n", "File Name", "File Type");
    printf("----------------------------------------------\n");

    // Read and process each line in the file
    while (fgets(line, sizeof(line), file))
    {
        line_count++;

        // Skip the first line (header) and lines containing "main"
        if (line_count == 1 || strstr(line, "main") != NULL)
        {
            continue;
        }

        char file_type = line[0];
        char *file_name = strrchr(line, ' ');

        if (file_name != NULL)
        {
            file_name++;
            char *newline = strchr(file_name, '\n');
            if (newline != NULL)
            {
                *newline = '\0';
            }

            // Print file name and file type in a table format
            printf("   %-25s%-20s\n", file_name, check_file_type(file_type));
        }
    }

    fclose(file);
}

void get_sub_file(const char *file_name)
{
    char command[MAX_FILE_NAME];
    char *temp_file = "temp_file.txt";
    snprintf(command, sizeof(command), "ls -l \"%s\" > %s", file_name, temp_file);
    int result = system(command);
    if (result != 0)
    {
        perror("Error executing command");
    }
    format_data_from_dir(temp_file);
    system("rm -rf *.txt");
}
