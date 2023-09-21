#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_NAME 256

// Function to check and return the file type as a string
const char* check_file_type(char file_type) {
    switch (file_type) {
        case '-':
            return "Regular or ordinary file";
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

// Function to format and display data from the directory listing
void format_data_from_dir(const char *file_name) {
    FILE *file = fopen(file_name, "r");

    if (file == NULL) {
        perror("Error opening the file");
        return;
    }

    char line[MAX_FILE_NAME];
    int line_count = 0;

    // Print table header
    printf("%-40s%-30s\n", "File Name", "File Type");
    printf("--------------------------------------------------------\n");

    // Read and process each line in the file
    while (fgets(line, sizeof(line), file)) {
        line_count++;

        // Skip the first line (header) and lines containing "main"
        if (line_count == 1 || strstr(line, "main") != NULL) {
            continue;
        }

        char file_type = line[0];
        char *file_name = strrchr(line, ' ');

        if (file_name != NULL) {
            file_name++;
            char *newline = strchr(file_name, '\n');
            if (newline != NULL) {
                *newline = '\0';
            }

            // Print file name and file type in a table format
            printf("%-40s%-30s\n", file_name, check_file_type(file_type));
        }
    }

    fclose(file);
}

int main() {
    format_data_from_dir("output.txt");
    return 0;
}
