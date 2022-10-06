#!/bin/bash

# delete existing files
rm *.txt

# single character files
tr -dc A-Za-z0-9 </dev/urandom | head -c 1 >single1.txt
tr -dc A-Za-z0-9 </dev/urandom | head -c 1 >single2.txt

# two long files
tr -dc A-Za-z0-9 </dev/urandom | head -c 1500 >long1.txt
tr -dc A-Za-z0-9 </dev/urandom | head -c 1500 >long2.txt

# two long files (only one character different)
tr -dc A-Za-z0-9 </dev/urandom | head -c 1500 >long-diff1.txt
cp long-diff1.txt long-diff2.txt
tr -dc A-Za-z0-9 </dev/urandom | head -c 1 >>long-diff1.txt
tr -dc A-Za-z0-9 </dev/urandom | head -c 1 >>long-diff2.txt

# empty file
touch empty.txt

# konstitucija
curl --http1.1 -L "https://www.dropbox.com/s/ce83ry9a4d9642z/konstitucija.txt?dl=1" -o konstitucija.txt

# generate 25000 string pairs with different length
cat /dev/urandom | tr -dc '[:alpha:]' | fold -w ${1:-10} | head -n 50000 >>100000-pairs.txt
cat /dev/urandom | tr -dc '[:alpha:]' | fold -w ${1:-100} | head -n 50000 >>100000-pairs.txt
cat /dev/urandom | tr -dc '[:alpha:]' | fold -w ${1:-500} | head -n 50000 >>100000-pairs.txt
cat /dev/urandom | tr -dc '[:alpha:]' | fold -w ${1:-1000} | head -n 50000 >>100000-pairs.txt

# generate 25000 strings with different length
cat /dev/urandom | tr -dc '[:alpha:]' | fold -w ${1:-10} | head -n 25000 >>100000-lines.txt
cat /dev/urandom | tr -dc '[:alpha:]' | fold -w ${1:-100} | head -n 25000 >>100000-lines.txt
cat /dev/urandom | tr -dc '[:alpha:]' | fold -w ${1:-500} | head -n 25000 >>100000-lines.txt
cat /dev/urandom | tr -dc '[:alpha:]' | fold -w ${1:-1000} | head -n 25000 >>100000-lines.txt

# for additional testing
# 50kb
tr -dc A-Za-z0-9 </dev/urandom | head -c 50000 >short.txt
# 2mb
tr -dc A-Za-z0-9 </dev/urandom | head -c 2000000 >long.txt
# 20mb
tr -dc A-Za-z0-9 </dev/urandom | head -c 20000000 >very-long.txt
# 200mb
tr -dc A-Za-z0-9 </dev/urandom | head -c 200000000 >even-longer.txt
