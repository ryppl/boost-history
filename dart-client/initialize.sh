#!/bin/sh -x

#
# initialize.sh
# 

# This script initializes a dart-client testing hierarchy on local
# disk to support running the run.sh script found in this directory.

# The directory structure constructed by this script is as follows:
#
# debug/
#   nightly/
#     src/
#     build/
#   continuous/
#     src/
#     build/
# release/
#   nightly/
#     src/
#     build/
#   continuous/
#     src/
#     build/    

#
# Customizations:
# 
# srcurl:  The source to build and test
#
srcurl="http://svn.boost.org/svn/boost/sandbox-branches/boost-cmake/boost_1_34_0"

#
# path to cmake
#
cmake_bin="cmake"

#
# path to svn
#
svn_bin="svn"

#
# CXX: if you'd like to specify a particular compiler, set it here
#
export CXX=

#
# Set topdir to directory where the builds should go.  Default is the directory
# where this script is found.
#
scriptdir=`dirname $0`
cd $scriptdir
topdir=`pwd`

# 
# recheckout to local copy
#
rm -rf src.tmp
$svn_bin co $srcurl src.tmp

for variant in debug release
do
  for rate in nightly continuous
  do
    builddir=$topdir/$variant/$rate
    rm -rf $builddir
    mkdir -p $builddir/build
    # instead of doing multiple checkouts and abusing the svn box, just rsync your temporary
    rsync -a $topdir/src.tmp/ $builddir/src/
    cd $builddir/build
    rm -f CMakeCache.txt
    if [ "$variant" = "debug" ] ; then
	VARIANT_ARGS="-DBUILD_DEBUG:BOOL=ON -DBUILD_RELEASE:BOOL=OFF"
    else
	VARIANT_ARGS="-DBUILD_DEBUG:BOOL=OFF -DBUILD_RELEASE:BOOL=ON"
    fi
    CXX="$COMPILER" $cmake_bin \
	-DBUILD_TESTING:BOOL=ON \
	-DCMAKE_BUILD_TYPE:STRING= \
	-DCMAKE_VERBOSE_MAKEFILE:STRING=ON \
	$VARIANT_ARGS $builddir/src
  done
done

echo "Done initializing dart-client build directories."

