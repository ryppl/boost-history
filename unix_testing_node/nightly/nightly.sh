#!/bin/sh -x

topdir=`dirname $0`
cd $topdir
topdir=`pwd`

cd $topdir/debug
ctest -S nightly.cmake -V
cd $topdir/release
ctest -S nightly.cmake -V

