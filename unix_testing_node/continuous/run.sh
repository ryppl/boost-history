#!/bin/sh -x

topdir=`pwd`

# alternately build debug and release
clean_dirs () {
    echo "cleaning dirs"
    for builddir in debug release
      do
      dir = $topdir/$builddir/1.34.0/build
      rm -rf $dir
      mkdir -p $dir
      cd $dir
      cmake ../src
    done
}

do_builds () {
    echo "Doing builds..."
    cd $topdir/debug/1.34.0/build
    ctest -D Continuous
    cd $topdir/release/1.34.0/build
    ctest -D Continuous
}

COUNT=0

while true
do
#  sleep 180  # wait three minutes
  COUNT=`expr $COUNT + 1`
  echo "COUNT = $COUNT"
  do_builds
  if [ $COUNT -eq 5 ]
      then
      clean_dirs
      COUNT=0
  fi
done

