#!/bin/bash
# Check if two command-line arguments are provided
if [ $# -ne 3 ]; then
    echo "Usage: $0 <num1> <operator> <num3>"
    exit 1
fi
# Retrieve the two numbers from command-line arguments
num1="$1"
operator="$2"
num2="$3"
# Perform the operation based on the operator
if [ "$operator" = "+" ]; then
    result=$((num1 + num2))
    operation="addition"
elif [ "$operator" = "-" ]; then
    result=$((num1 - num2))
    operation="subtraction"
elif [ "$operator" = "x" ]; then
    result=$((num1 * num2))
    operation="multiplication"
elif [ "$operator" = "/" ]; then
    result=$((num1 / num2))
    operation="division"
else
    echo "Invalid operator: $operator"
    exit 1
fi

# Display the result and operation
echo "The $operation of $num1 $operator $num2 is: $result"
