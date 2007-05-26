#!/bin/sh -x

if [[ `svn status $0` ]] 
then
    svn update $0
    $0
    exit
fi

topdir=`dirname $0`
cd $topdir
topdir=`pwd`

cd $topdir/debug
CXX="ccache gcc" ctest -S nightly.cmake -V
cd $topdir/release
CXX="ccache gcc" ctest -S nightly.cmake -V



