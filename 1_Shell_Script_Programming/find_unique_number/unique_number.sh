#!/bin/bash

while true; do
    read -p "Enter the number of integers (must be odd): " N
    if [[ $N =~ ^[1-9][0-9]*$ && $((N % 2)) -eq 1 ]]; then
        break
    else
        echo "Invalid input. Please enter a positive odd number."
    fi
done

read -p "Enter $N space-separated integers: " numbers

# Use tr to replace spaces with newlines
num_array=$(echo "$numbers" | tr ' ' '\n')

# Use sort and uniq to find unique numbers
unique_numbers=$(echo "$num_array" | sort | uniq -u)

# Use awk to format the result with spaces
unique_numbers=$(echo "$unique_numbers" | awk '{printf("%s ", $0)}')

# Trim trailing whitespace
unique_numbers="${unique_numbers%"${unique_numbers##*[![:space:]]}"}"

echo "$unique_numbers"

