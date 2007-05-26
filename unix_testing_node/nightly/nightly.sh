#!/bin/sh -x

if [[ -z "$UPDATE_DONE" ]] 
then
    svn update $0
    UPDATE_DONE=YES $0 $*
    exit
fi

COMPILER=$1
if [[ -z "$COMPILER" ]]
then
    echo "Setting compiler to default of g++"
    COMPILER=g++
fi

topdir=`dirname $0`
cd $topdir
topdir=`pwd`

cd $topdir/debug
CXX="$COMPILER" ctest -S nightly.cmake -V
cd $topdir/release
CXX="$COMPILER" ctest -S nightly.cmake -V



