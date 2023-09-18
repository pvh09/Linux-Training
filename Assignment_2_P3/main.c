#include "file_handling.h"

int main(){
    char file_name[MAX_FILE_NAME];
    input_file(file_name);

    create_hard_link_function(file_name);

    create_soft_link_function(file_name);
}