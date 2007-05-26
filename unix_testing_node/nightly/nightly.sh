#!/bin/sh -x

if [[ -z "$UPDATE_DONE" ]] 
then
    svn update $0
    UPDATE_DONE=YES $0
    exit
fi

topdir=`dirname $0`
cd $topdir
topdir=`pwd`

cd $topdir/debug
CXX="ccache g++" ctest -S nightly.cmake -V
cd $topdir/release
CXX="ccache g++" ctest -S nightly.cmake -V



