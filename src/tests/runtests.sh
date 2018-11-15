#!/bin/sh

export SH=../../42sh

RED='\033[0;31m'
GREEN='\033[0;32m'
END='\e[0m'

for TEST in *.sh; do
    if ./$TEST; then 
        printf "$GREEN$TEST passed$END\n"
    else
        printf "$RED$TEST failed$END\n"
        exit 1
    fi
done
