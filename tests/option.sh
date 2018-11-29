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
while getopts "lstc:" opt
do
  case "$opt" in
    "l")
          cd test_yml
          for folder in *; do
            echo $folder
          done;;
    "c") echo "category $OPTARG";;
    "s") echo "sanity";;
    "t") echo "timeout";;
  esac
done
shift $(expr $OPTIND - 1)