#!/bin/sh -x

topdir=`pwd`
COMPILER=$1
if [[ -z "$COMPILER" ]]
then
    echo "No compiler specified, using g++"
    COMPILER=g++
fi

cd $topdir/debug/1.34.0/build
rm CMakeCache.txt
CXX="$COMPILER" cmake -DBUILD_TESTING:BOOL=ON -DCMAKE_BUILD_TYPE:STRING=Debug ../src

cd $topdir/release/1.34.0/build
rm CMakeCache.txt
CXX="$COMPILER" cmake -DBUILD_TESTING:BOOL=ON -DCMAKE_BUILD_TYPE:STRING=Release ../src

