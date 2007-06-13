#!/bin/sh -x

#
# run.sh:  run nightly and continuous builds.
#

#
# This script runs as a daemon.  Once per day a 'nightly' build will
# be run and the continuous build directories will be cleaned out.
# After this 'continuous' builds will be continuously run.
# 

#
# Set topdir to directory where the builds should go.  Default is the directory
# where this script is found.
#
scriptdir=`dirname $0`
cd $scriptdir
topdir=`pwd`

#
# Start script
#

# date of last nightly build: YYYYMMDD
lastnightly=00000000 
sleepduration=180
ctest_bin=ctest
# alternately build debug and release
while true
do
  rightnow=`date +%Y%m%m`
  if [ $rightnow -gt $lastnightly ] ; then
      lastnightly=$rightnow

      # do nightly builds
      cd $topdir/debug/nightly/build
      $ctest_bin -D Nightly
      cd $topdir/release/nightly/build
      $ctest_bin -D Nightly
  fi

  cd $topdir/debug/continuous/build
  $ctest_bin -D Continuous
  cd $topdir/release/continuous/build
  $ctest_bin -D Continuous

  sleep $sleepduration
done

