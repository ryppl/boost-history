#!/bin/sh -x

topdir=`pwd`

# alternately build debug and release
while true
do
  cd $topdir/debug/1.34.0/build
  ctest -D Continuous
  cd $topdir/release/1.34.0/build
  ctest -D Continuous
  sleep 180  # wait three minutes
done

