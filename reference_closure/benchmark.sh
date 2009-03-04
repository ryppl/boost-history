#!/bin/bash
set -e
CC=g++

echo compile trivial_nested
$CC -O2 -DREFERENCE_CLOSURE trivial_nested.cc -o trivial_nested
echo compile trivial_function
$CC -O2 -DSTD_FUNCTION trivial_function.cc -o trivial_function

echo
echo -n execute trivial_nested
time ./trivial_nested

echo
echo -n execute trivial_function
time ./trivial_function

