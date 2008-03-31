#!/bin/sh

NAME=unordered-addon
TARBALL_DIR=~/tarballs
UNORDERED_DST=$TARBALL_DIR/$NAME
TARBALL=$TARBALL_DIR/$NAME.tar.gz
ZIPFILE=$TARBALL_DIR/$NAME.zip

mkdir -p $TARBALL_DIR
rm $TARBALL
rm $ZIPFILE
rm -rf $UNORDERED_DST

svn export . $UNORDERED_DST

cd $UNORDERED_DST/libs/unordered/doc
bjam
cd -

rm -r $UNORDERED_DST/libs/functional
rm -r $UNORDERED_DST/bin.v2
rm $UNORDERED_DST/release.sh

cd $TARBALL_DIR
tar -czf $TARBALL $NAME
zip -r $ZIPFILE $NAME
cd -

rm -r $UNORDERED_DST

