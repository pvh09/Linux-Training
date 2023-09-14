#!/bin/bash

# fruits=("blueberry" "peach" "mango" "pineapple" "papaya") 

# for n in ${fruits[@]}; 
# do
#     echo $n
# done

#!/bin/bash

for (( n=1; n<=7; n++ ))
do  
    # Check if the number is even or not
    if (( $n%2==0 ))
    then
        echo "$n is even"
    else
        echo "$n is odd"
    fi
done