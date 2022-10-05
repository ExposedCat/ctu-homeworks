#!/bin/bash

# Compile
echo ">>> Compiling"
clang -pedantic -Wall -Werror -std=c99 -O2 -lm -g main.c -o main

# Test
echo ">>> Testing"
mkdir -p out
TESTS_NUMBER=$(command ls -1q data/pub*.in | wc -l)
echo ">>> Found $TESTS_NUMBER tests"

i=1
cd ./public
for file in *.in; do
    echo ">>>> Running test $i"
    ID=$(echo $file | sed 's/pub\(.*\)\..*/\1/')
    ../main < ./pub$ID.in 2> ../out/my$ID.err 1> ../out/my$ID.out
    # Show difference
    echo ">>> Result:"
    diff ./pub$ID.out ../out/my$ID.out\
        --old-line-format=$'- %L\n'\
        --new-line-format=$'+ %L\n'\
        --unchanged-line-format='' && echo "OUT OK"
    ((i=i+1))
done
