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
timeout=0
category=""
while getopts "lstc:" opt
do
  case "$opt" in
    "l")
          cd test_yml
          for folder in *; do
            echo $folder
          done;;
    "c") category+="$OPTARG "; echo "category $OPTARG";;
    "s") sanity=1; echo "sanity";;
    "t") timeout=1; echo "timeout";;
  esac
done

if [ $category = "" ]; then
  if [ $sanity -eq 1 ]; then
      valgrind ./loadtest.sh
  else
      ./loadtest.sh
  fi
else
  for test in $category; do
    if [ $sanity -eq 1 ]; then
      valgrind ./loadtest.sh $test
    else
      ./loadtest.sh $test
    fi 
  done
fi

shift $(expr $OPTIND - 1)