#!/bin/sh
CC=g++

echo compile trivial_nested
time $CC -O2 trivial_nested.cc -o trivial_nested
echo execute trivial_nested
time ./trivial_nested

echo compile trivial_function
time $CC -O2 trivial_function.cc -o trivial_function
echo execute trivial_function
time ./trivial_function

