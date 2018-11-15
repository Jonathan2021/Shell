#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
END='\e[0m'

T1=$(mktemp)
t2=$(mktemp)
VAL='valgrind --leak-check=full --show-leak-kinds=all'

while read args; do
    $VAL $SH < $args | sort > $T1
    $args | sort > $T2
    if diff $T1 $T2; then
        printf "$GREEN$args$END\n"
    else
        printf "$RED$arg$END\n"
        exit 1
    fi
done <<EOF

if pwd && ls; then 
    cd 
else 
    alias
fi
EOF

rm $T1 $T2
