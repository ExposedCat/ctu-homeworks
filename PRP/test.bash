#!/bin/bash

# Compile
echo ">>> Compiling"
clang -pedantic -Wall -Werror -std=c99 -O2 -g main.c -o main

# Test
echo ">>> Testing"
mkdir -p out
TESTS_NUMBER=$(command ls -1q data/pub*.in | wc -l)
for ((i=1;i<=TESTS_NUMBER;i++)); do
    echo ">>>> Running test $i";
    ./main < data/pub0$i.in > out/my-0$i.out;
    # Show difference
    echo ">>> Result:";
    diff data/pub0$i.out out/my-0$i.out && echo "OK"
done
