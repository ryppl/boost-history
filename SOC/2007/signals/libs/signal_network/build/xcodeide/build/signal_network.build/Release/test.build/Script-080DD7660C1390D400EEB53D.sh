#!/bin/sh
# shell script goes here
style=`echo $BUILD_STYLE | sed 'y/DR/dr/'`  
bjam --v2 --toolset=darwin ../../test $style
