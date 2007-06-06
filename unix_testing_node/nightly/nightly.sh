#!/bin/sh -x

topdir=`dirname $0`
bin=`basename $0`
cd $topdir
topdir=`pwd`

if [ -z "$UPDATE_DONE" ] 
then
    export UPDATE_DONE=YES
    svn update $bin
    exec $0 $*
fi

COMPILER=$1
if [ -z "$COMPILER" ]
then
    echo "Setting compiler to default of g++"
    COMPILER=g++
fi

cd $topdir/debug
CXX="$COMPILER" ctest -S nightly.cmake -V
cd $topdir/release
CXX="$COMPILER" ctest -S nightly.cmake -V



