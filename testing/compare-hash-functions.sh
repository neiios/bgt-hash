#!/bin/bash

echo "Igor Repkin CHM"
cd ..
make

echo "long"
perf stat -r 10 -B ./bin -f testing/long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "very long"
perf stat -r 10 -B ./bin -f testing/very-long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "even longer"
perf stat -r 10 -B ./bin -f testing/even-longer.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'

echo "Maksim Barbasov MYSHA"
cd hash-generator
g++ src/*.cpp lib/hash-library/{sha256,md5,sha1,keccak}.cpp -o 'bin'

echo "long"
perf stat -r 10 -B ./bin ../testing/long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "very long"
perf stat -r 10 -B ./bin ../testing/very-long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "even longer"
perf stat -r 10 -B ./bin ../testing/even-longer.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'

echo "Markas Vielavicius BCE"
cd ../BlockChainsExercises
mkdir -pv build
cd build
cmake -B . -S ..
make

echo "long"
perf stat -r 10 -B ./HashAlgorithm -f ../../testing/long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "very long"
perf stat -r 10 -B ./HashAlgorithm -f ../../testing/very-long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "even longer"
perf stat -r 10 -B ./HashAlgorithm -f ../../testing/even-longer.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'

echo "Deividas Roščenkovas BCP"
cd ../../Blockchain-Pratybos
make

echo "long"
perf stat -r 10 -B ./prog ../testing/long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "very long"
perf stat -r 10 -B ./prog ../testing/very-long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "even longer"
perf stat -r 10 -B ./prog ../testing/even-longer.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'

echo "Eimantas Katinas HG"
cd ../hash-generatorius
make

echo "long"
perf stat -r 10 -B ./hash -f ../testing/long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "very long"
perf stat -r 10 -B ./hash -f ../testing/very-long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
echo "even longer"
perf stat -r 10 -B ./hash -f ../testing/even-longer.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'

echo "Liudvikas Radvilavičius JAVAHASH"
cd ../2K-3S-BGT
javac src/lt/vu/mif/stud/*.java

cd src
# echo "long"
# perf stat -r 10 -B java lt.vu.mif.stud.Main -f ../../testing/long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
# echo "very long"
# perf stat -r 10 -B java lt.vu.mif.stud.Main -f ../../testing/very-long.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
# echo "even longer"
# perf stat -r 10 -B java lt.vu.mif.stud.Main -f ../../testing/even-longer.txt 2>&1 >/dev/null | tail -n 2 | sed 's/ \+//' | sed 's/ /,/'
