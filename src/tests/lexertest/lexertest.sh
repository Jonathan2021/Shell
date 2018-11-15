#!/bin/sh

while read line; do
    echo $line
    $(echo $line | ./$1 > TST)
    res=$(grep -n "Result: 1" TST)
    if [ -n "$res" ]; then
        $(echo $line > /dev/null)
        err=$(echo $?)
        if [ $err -eq 0 ] ; then
            echo "OK"
        else
            echo "Something goes wrong"
        fi
    else
        echo "ko"
    fi
done < command
rm TST
