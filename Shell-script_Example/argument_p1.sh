#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <arg1> [arg2] [arg3] ..."
    exit 1
fi

echo "Total number of arguments: $#"

# Access and display individual arguments
echo "First argument: $1"
echo "Second argument: $2"
echo "Third argument: $3"

# Loop through all arguments
echo "All arguments:"
for arg in "$@"; do
    echo "$arg"
done
