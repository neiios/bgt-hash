#!/bin/bash

echo "Igor Repkin CHM"
cd ..
make

echo "short"
perf stat -r 5 -B ./bin -f testing/short.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "long"
perf stat -r 5 -B ./bin -f testing/long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "very long"
perf stat -r 5 -B ./bin -f testing/very-long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "even longer"
perf stat -r 5 -B ./bin -f testing/even-longer.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'

echo "Maksim Barbasov MYSHA"
cd hash-generator
g++ src/*.cpp lib/hash-library/{sha256,md5,sha1,keccak}.cpp -o 'bin'

echo "short"
perf stat -r 5 -B ./bin ../testing/short.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "long"
perf stat -r 5 -B ./bin ../testing/long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "very long"
perf stat -r 5 -B ./bin ../testing/very-long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "even longer"
perf stat -r 5 -B ./bin ../testing/even-longer.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'

echo "Markas Vielavicius BCE"
cd ../BlockChainsExercises
mkdir -pv build
cd build
cmake -B . -S ..
make

echo "short"
perf stat -r 5 -B ./HashAlgorithm -f ../../testing/short.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "long"
perf stat -r 5 -B ./HashAlgorithm -f ../../testing/long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "very long"
perf stat -r 5 -B ./HashAlgorithm -f ../../testing/very-long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "even longer"
perf stat -r 5 -B ./HashAlgorithm -f ../../testing/even-longer.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'

echo "Deividas Roščenkovas BCP"
cd ../../Blockchain-Pratybos
make

echo "short"
perf stat -r 5 -B ./prog ../testing/short.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "long"
perf stat -r 5 -B ./prog ../testing/long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "very long"
perf stat -r 5 -B ./prog ../testing/very-long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "even longer"
perf stat -r 5 -B ./prog ../testing/even-longer.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'

echo "Eimantas Katinas HG"
cd ../hash-generatorius
make

echo "short"
perf stat -r 5 -B ./hash -f ../testing/short.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "long"
perf stat -r 5 -B ./hash -f ../testing/long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "very long"
perf stat -r 5 -B ./hash -f ../testing/very-long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "even longer"
perf stat -r 5 -B ./hash -f ../testing/even-longer.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'

echo "Liudvikas Radvilavičius JAVAHASH"
cd ../2K-3S-BGT
javac src/lt/vu/mif/stud/*.java

cd src
# special file for the java enjoyer
echo "short"
tr -dc A-Za-z0-9 </dev/urandom | head -c 50000 >short.txt
perf stat -r 1 -B java lt.vu.mif.stud.Main -f short.txt | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
# echo "long"
# perf stat -r 5 -B java lt.vu.mif.stud.Main -f ../../testing/long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
# echo "very long"
# perf stat -r 5 -B java lt.vu.mif.stud.Main -f ../../testing/very-long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
# echo "even longer"
# perf stat -r 5 -B java lt.vu.mif.stud.Main -f ../../testing/even-longer.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
