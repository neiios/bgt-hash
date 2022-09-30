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

# create a graph from -b output
# cat data.txt | awk '{ print $2 " " $5}' | gnuplot -p -e 'set key bottom right; set format y "%.5f"; set xrange [0:520]; set terminal png size 400,300; set output "graph.png"; set grid; plot "/dev/stdin" w lp lw 4 title "Time taken"'
