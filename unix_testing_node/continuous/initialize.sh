#!/bin/sh -x

topdir=`pwd`
COMPILER=$1
if [[ -z "$COMPILER" ]]
then
    print "No compiler specified, using g++"
    COMPILER=g++
fi

cd $topdir/debug/1.34.0/src
svn update
cd $topdir/debug/1.34.0/build
rm CMakeCache.txt
CXX="ccache $COMPILER" cmake -DBUILD_TESTING:BOOL=ON -DCMAKE_BUILD_TYPE:STRING=Debug ../src

cd $topdir/release/1.34.0/src
svn update
cd $topdir/release/1.34.0/build
rm CMakeCache.txt
CXX="ccache $COMPILER" cmake -DBUILD_TESTING:BOOL=ON -DCMAKE_BUILD_TYPE:STRING=Release ../src

