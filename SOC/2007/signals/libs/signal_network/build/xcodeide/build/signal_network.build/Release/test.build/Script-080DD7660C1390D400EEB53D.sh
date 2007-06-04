#!/bin/sh
# shell script goes here
echo warning: $BUILD_VARIANTS
bjam --v2 --toolset=darwin ../../test 
