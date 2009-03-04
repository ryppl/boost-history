#!/bin/bash
set -e
CC=g++

set -x
$CC -O2 -DREFERENCE_CLOSURE $CXXFLAGS parallel_lib.cc -c -o parallel_lib_refclosure.o
$CC -O2 -DREFERENCE_CLOSURE $CXXFLAGS main.cc -c -o main_refclosure.o
$CC main_refclosure.o parallel_lib_refclosure.o -o refclosure

$CC -O2 -DSTD_FUNCTION $CXXFLAGS parallel_lib.cc -c -o parallel_lib_stdfunction.o
$CC -O2 -DSTD_FUNCTION $CXXFLAGS main.cc -c -o main_stdfunction.o
$CC main_stdfunction.o parallel_lib_stdfunction.o -o stdfunction

set +x

echo
echo -n execute refclosure
time ./refclosure

echo
echo -n execute stdfunction
time ./stdfunction

