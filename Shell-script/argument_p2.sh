#!/bin/bash

# Kiểm tra xem đủ hai đối số chưa (tệp nguồn và tệp đích)
if [ $# -ne 2 ]; then
    echo "Usage: $0 <source_file> <destination_file>"
    exit 1
fi

#!/bin/bash

source_file="$1"
destination_file="$2"

if [ ! -f "$source_file" ]; then
    echo "Error: Source file '$source_file' does not exist."
    exit 1
fi

if [ -e "$destination_file" ]; then
    echo "Error: Destination file '$destination_file' already exists."
    exit 1
fi

cp "$source_file" "$destination_file"
if [ $? -eq 0 ]; then
    echo "File '$source_file' copied to '$destination_file' successfully."
else
    echo "Error copying file '$source_file' to '$destination_file'."
fi
