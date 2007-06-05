#!/bin/sh -x

topdir=`pwd`

# alternately build debug and release
clean_dirs () {
    echo "cleaning dirs"
    for builddir in debug release
      do
      dir=$topdir/$builddir/1.34.0/build
      rm -rf $dir
      mkdir -p $dir
      cd $dir
      $topdir/initialize.sh
    done
}

do_builds () {
    echo "Doing builds..."
    cd $topdir/debug/1.34.0/build
    ctest -D Continuous
    cd $topdir/release/1.34.0/build
    ctest -D Continuous
}

while true
do
  do_builds
  if [ `date +%H` -eq 0 ]
      then
      clean_dirs
  fi
  sleep 300 # wait 5 minutes
done

