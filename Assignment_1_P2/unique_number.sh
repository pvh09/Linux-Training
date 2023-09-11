#!/bin/bash
while true
do
    read -p "Enter the number of integers (must be odd): " N
    if [[ $N =~ ^[1-9][0-9]*$ && $((N % 2)) -eq 1 ]]; then
        break
    else
        echo "Invalid input. Please enter a positive odd number."
    fi
done

read -p "Enter $N space-separated integers: " A

A=($A)

# Loop through the array and process each integer
for ((i = 0; i < N; i++)); do
    echo "Integer $i: ${A[i]}"
done
