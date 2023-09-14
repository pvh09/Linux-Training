#!/bin/bash

a=100
b=10

if(( $a == $b ))
then
  echo "a and b are equal"
fi

if(( $a != $b ))
then
  echo "a and b are not equal"
fi

if(( $a > $b ))
then
  echo "a is greater than b"
else
  echo "a is not greater than b"
fi

if(( $a >= $b ))
then
  echo "a is greater or equal to than b"
else
  echo "a is not greater or equal to than b"
fi

