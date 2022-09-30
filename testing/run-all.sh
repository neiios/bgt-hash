#!/bin/bash

# generate new files if -g cli argument is passed
while getopts "g" opt; do
  case "$opt" in
  g)
    ./generate-files.sh
    ;;
  esac
done

# compile the binary
cd .. && make && cd testing

# run tests
source test-2.sh
source final-tests.sh
