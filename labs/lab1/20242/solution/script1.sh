#!/bin/bash
# Saymon N. - 20211866
num1=$1
num2=$2

for ((k=num1;k<=num2;k++))
do
    echo -n $k
    num=$k
    factors=`factor $num | wc -w`
    if [ $factors -eq 2 ]; then
        echo -n " (prime)"
    fi
    echo ""
done
