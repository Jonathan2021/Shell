#!/bin/sh

if [ $# -eq 1 ] ; then
    LS=$(find .  | grep "\.yml" | grep "^\.")
    $1 $LS
    exit 0
fi

if [ -d $2 ] ; then
    LS=$(ls $2)
    $1 $2
    exit 0
fi

if [ -f $2 ] ; then
    $1 $2
    exit 0
fi
