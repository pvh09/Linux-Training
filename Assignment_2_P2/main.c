#include "file_handling.h"

int main()
{
    char output_file_name[MAX_FILE_NAME];
    create_output_file(output_file_name);
    enter_and_read_from_keyboard(output_file_name);
    return 0;
}