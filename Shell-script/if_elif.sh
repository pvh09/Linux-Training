#!/bin/bash

# echo "Enter filename"
# read filename

# if [ -e $filename ]
# then
# echo "$filename is exits on the directory"
# cat $filename

# else
#     cat > $filename
#     echo "File created"
# fi

#====================
# # -eq
# if [ 10 -eq 10 ];then
# echo "Equal"
# fi

# # -ge
# if [ 10 -ge 9 ];then
# echo "Greater or equal"
# fi

# # -gt
# if [ 10 -gt 8 ];then
# echo "Greater"
# fi

# # -le
# if [ 10 -le 12 ];then
# echo "Less than or equal"
# fi

# # -lt
# if [ 10 -lt 13 ];then
# echo "Less than"
# fi

# # -ne
# if [ 10 -ne 13 ];then
# echo "Not Equal"
# fi

#=====================================
# ==
if [ 'check' == 'check' ];
then
    echo "same" #output
else
    echo "not same"
fi

# !=
if [ 'check' != 'Apple' ];
then
    echo "not same" #output
else
    echo "same"
fi

#!/bin/bash

str1=""
if [ -n "$str1" ]; then
    echo "string not empty: '$str1'"
else
    echo "empty"
fi

str2=""
if [ -z "$str2" ]; then
    echo "string not empty: '$str2'"
else
    echo "empty"
fi

