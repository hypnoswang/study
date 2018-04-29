#! /bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: /bin/sh random_array.sh <max> <count>"
    exit 1
fi

max=$1
count=$2

i=0
while [ $i -lt $count ]
do
    sym=$(($RANDOM % 2))
    num=$(($RANDOM % $max))
    if [ $sym -ne 1 ]; then
        num=$((0 - $num))
    fi

    echo -n $num","

    i=$(($i + 1))
done
