#!/bin/bash

echo "Testing if the hash is the same length length and hash of the same string is the same:"

echo "------------------- Test case 1 single character-------------------"
echo "First file:"
../bin -f single1.txt -f single1.txt
echo "Second file:"
../bin -f single2.txt -f single2.txt
echo "------------------- Test case 2 long file -------------------"
echo "First file:"
../bin -f long1.txt -f long1.txt
echo "Second file:"
../bin -f long2.txt -f long2.txt
echo "------------------- Test case 3 one different character ------------------"
echo "First file:"
../bin -f long-diff1.txt -f long-diff1.txt
echo "Second file:"
../bin -f long-diff2.txt -f long-diff2.txt
echo "------------------- Test case 4 empty file -------------------"
echo "First file:"
../bin -f empty.txt -f empty.txt

