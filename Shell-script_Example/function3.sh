#!/bin/bash
is_odd() {
  x=$1
  if [ $((x % 2)) == 0 ]; then
     echo "Invalid Input"
     exit 1
  else
     echo "Number is Odd"
  fi
}

# Truyền đối số từ dòng lệnh
is_odd "$1"
