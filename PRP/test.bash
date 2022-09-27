#!/bin/bash

# Compile
echo ">>> Compiling"
clang -pedantic -Wall -Werror -std=c99 -O2 -g main.c -o main

# Test
echo ">>> Testing"
mkdir -p out
TESTS_NUMBER=$(command ls -1q data/pub*.in | wc -l)
echo ">>> Found $TESTS_NUMBER tests"
for ((i=1;i<=TESTS_NUMBER;i++)); do
    echo ">>>> Running test $i";
    ./main < data/pub0$i.in 2> out/my-0$i.err 1> out/my-0$i.out;
    # Show difference
    echo ">>> Result:";
    diff data/pub0$i.out out/my-0$i.out\
        --old-line-format=$'- %L'\
        --new-line-format='+ %L'\
        --unchanged-line-format='' && echo "OUT OK";
        
    diff data/pub0$i.err out/my-0$i.err\
        --old-line-format=$'- %L'\
        --new-line-format='+ %L'\
        --unchanged-line-format='' && echo "ERR OK";
done
