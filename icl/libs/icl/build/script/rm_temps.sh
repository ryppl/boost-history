#!/bin/bash
echo "removing $2 from $1"
rm -rf `find $1 -name $2` ;