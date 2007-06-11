#!/bin/sh

###########################################################################
# Simple Wrapper for mfront
# Directions:
#  - Choose which configuration file to use (or make your own)
#  - Configure the path of your mfront.bin (exe) binary
#  - Configure the path of your c++ compiler
#  - Put this in your path
###########################################################################
BASE=`which $0`
BASE=`dirname $BASE`

#
# Mac OS X Configuration
CONFIGFILE="$BASE/params.macosx"
MFRONTPATH="$BASE/../bin/darwin-3.2/debug/mfront_bin"
CXXPATH=`which g++`

echo "Configuration"
echo "-------------"
echo "BASE=$BASE"
echo "CONFIGFILE=$CONFIGFILE"
echo "MFRONTPATH=$MFRONTPATH"
echo "CXXPATH=$CXXPATH"
$MFRONTPATH @$CONFIGFILE -o - $* | $CXXPATH -x c++

