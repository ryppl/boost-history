#!/bin/sh
# (C) Copyright 2003-2004: Reece H. Dunn
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

##### Configuration

# tools="msvc-7.1 msvc-7.0 msvc-6.0 borland"

tools="borland msvc-6.0"

##### [1]: Perform the tests

for tool in $tools ; do      # for each toolset
   ##### [1a]: Build

   echo testing outfmt for $tool...
   bjam -a $tool 2>&1 | tee build.$tool.log > nul

   ##### [1b]: Run

   cd run                    #    go into the directory with the built tools
   for exec in *.exe ; do    #    for each test program
      ./$exec                #       run it!
   done
   cd ..                     #    go back to the build directory :)
done

##### [2]: Post-Test Clean Up

mv run/results.xml results.xml
rm -r run
rm -r bin

