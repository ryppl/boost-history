#!/bin/sh

tools="gcc msvc"

for tool in $tools ; do     # for each toolset
   echo testing for: $tool  #    inform the user we are testing this toolset
   bjam $tool               #    rebuild the regression tests

   cd run                   #    go into the directory with the built tools
   for exec in *.exe ; do   #    for each test program
      ./$exec               #       run it!
   done
   cd ..                    #    go back to the build directory :)
done
