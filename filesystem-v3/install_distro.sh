#!/usr/bin/env bash

# Install Boost Filesystem Version 3 distribution for POSIX
# © Copyright 2008 Beman Dawes
# Distributed under the Boost Software License, Version 1.0.
# See http://www.boost.org/LICENSE_1_0.txt

echo "Install Boost Filesystem Version 3 distribution for POSIX"
if [ $# -lt 1 ]
then
  echo "Usage: $0 distro-path boost-path"
  echo "Installs the Filesystem V3 distribution file found at distro-path"
  echo "in the Boost root found at boost-path" 
  exit 1
fi

if [ ! -e "$1" ]
then
  echo "error: $1 not found"
  exit 1
fi

if [ ! -e "$2" ]
then
  echo "error: $2 not found"
  exit 1
fi

if [ ! -e "$2/boost-build.jam" ]
then
  echo "error: $2 does not contain boost-build.jam"
  echo "Is it really a Boost root directory?"
  exit 1
fi

pushd $2
echo "Deleting prior version of filesystem library..."
rm boost/filesystem.hpp
rm -r boost/filesystem
rm -r libs/filesystem

if [ ! -e "bjam" ]
then
  echo "Running bootstrap script..."
  ./bootstrap.sh &>bootstrap.log
  echo "bootstrap complete - see $2/boostrap.log for details"
fi

popd
echo "Copying files from $1 to $2..."
cp -r $1/* $2
pushd $2

echo "Building libraries..."
date '+%T'
echo "If other libraries have not been previously built, this step may take a"
echo "long time - 10 minutes on a fast machine, much longer on a slow machine."
./bjam &>bjam.log
date '+%T'
echo "Library build complete - see $2/bjam.log for details."
echo "Libraries have been installed in $2/stage"

echo "Testing Filesystem Version 3..."
pushd libs/filesystem/test
date '+%T'
../../../bjam &>bjam.log
date '+%T'
popd
grep "...failed" libs/filesystem/test/bjam.log
echo "Testing complete - see $2/libs/filesystem/test/bjam.log for details."

echo "Installation complete."
popd
