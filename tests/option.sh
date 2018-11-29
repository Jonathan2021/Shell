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
          done; cd .. ; exit ;;
    "c") category+="$OPTARG "; ;;
    "s") sanity=1; ;;
    "t") timeout+=$OPTARG; timeout+="s";;
    *) exit; ;;
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
    if [ $sanity -eq 1 ] ; then
      eval $arg test_yml/$test > /dev/null
    else
      eval $arg test_yml/$test
    fi
  done
else
  echo $arg
  if [ $sanity -eq 1 ] ; then
    eval $arg > /dev/null
  else 
    eval $arg
  fi
fi

shift $(expr $OPTIND - 1)