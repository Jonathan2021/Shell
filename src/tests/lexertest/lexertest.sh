#!/bin/sh

while read line; do
    echo $line
    res1=$(echo $line | ./$1)
    res2
    echo $res1
done < command
