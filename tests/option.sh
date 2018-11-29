#!/bin/bash

for arg in "$@"; do
  shift
  case "$arg" in
    "--list") set -- "$@" "-l" ;;
    "--category") set -- "$@" "-c" ;;
    "--sanity")   set -- "$@" "-s" ;;
    "--timeout")   set -- "$@" "-t" ;;
    *)        set -- "$@" "$arg"
  esac
done

OPTIND=1
sanity=0
timeout=""
category=""
while getopts "lst:c:" opt
do
  case "$opt" in
    "l")
          cd test_yml
          for folder in *; do
            echo $folder
          done; cd .. ;;
    "c") category+="$OPTARG "; ;;
    "s") sanity=1; ;;
    "t") timeout+=$OPTARG; timeout+="s";;
  esac
done

arg=""

if [ $timeout ]; then
  arg+="timeout $timeout " 
fi

if [ $sanity -eq 1 ]; then
  arg+="valgrind "
fi
arg+="./loadtest.sh "
long=${#category}
if [ $long -ne 0 ]; then
  for test in $category; do
    echo $arg test_yml/$test
    eval $arg test_yml/$test
  done
else
  echo $arg
  eval $arg
fi

shift $(expr $OPTIND - 1)