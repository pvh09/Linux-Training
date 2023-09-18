#include "file_handling.h"

void create_output_file(char *output_file_name)
{
    bool is_file_exist = false;
    do
    {
        get_file_name(output_file_name);
        is_file_exist = is_over_range(output_file_name);

    } while (!is_file_exist || strlen(output_file_name) > MAX_FILE_NAME);
}

void enter_and_read_from_keyboard(char *output_file)
{
    int c;
    FILE *file = fopen(output_file, "w");

    if (file == NULL)
    {
        perror("Error opening the file");
        return;
    }

    printf("Enter data (press Ctrl+D on Linux or Ctrl+Z on Windows to finish):\n");

    printf("Enter text: ");
    while (1)
    {
        c = getchar();

        if (c == EOF)
        {
            break; // Exit the loop if Ctrl+D (Unix) or Ctrl+Z (Windows) is pressed
        }

        fputc(c, file); // Write the character to the file
    }

    fclose(file); // Close the file
    printf(">> Data is written into the file %s.\n", output_file);
}