#!/bin/sh

##### Configuration

tools="gcc msvc"

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

