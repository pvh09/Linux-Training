#! /bin/bash

# Take input from the user
echo -n "Enter a number"
read n

# Calculate remainder using mod (%) operator
remainder=$(( $n % 2 ))

# Odd or Even Check
if [ $remainder -eq 0 ]
then
 echo "You have entered $n -- which is an Even number"
else
 echo "You have entered $n -- which is an Odd number"
fi