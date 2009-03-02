#!/bin/sh

OUTPUT_NAME=boost-doc-tools
FILES=$(pwd)/files
TMPDIR=$(pwd)/tmp
DESTDIR=$(pwd)

ICONV=iconv-1.9.2.win32
LIBXSLT=libxslt-1.1.24.win32
LIBXML=libxml2-2.7.3.win32
ZLIB=zlib-1.2.3.win32
DOCBOOK_XSL=docbook-xsl-1.74.3
DOCBOOK_XML_VER=4.2

###############################################################################

OUTPUT_DIR=$TMPDIR/$OUTPUT_NAME

rm -r $TMPDIR
mkdir -p $OUTPUT_DIR/bin

cp $FILES/quickbook.exe $OUTPUT_DIR/bin

unzip -q -d $TMPDIR $FILES/$ICONV.zip
unzip -q -d $TMPDIR $FILES/$LIBXSLT.zip
unzip -q -d $TMPDIR $FILES/$LIBXML.zip
unzip -q -d $TMPDIR $FILES/$ZLIB.zip
unzip -q -d $TMPDIR $FILES/$DOCBOOK_XSL.zip

mv $TMPDIR/$LIBXML/readme.txt $OUTPUT_DIR/README-$LIBXML.txt
mv $TMPDIR/$LIBXSLT/readme.txt $OUTPUT_DIR/README-$LIBXSLT.txt

cp -r $TMPDIR/$ICONV/* $OUTPUT_DIR/
cp -r $TMPDIR/$LIBXSLT/* $OUTPUT_DIR/
cp -r $TMPDIR/$LIBXML/* $OUTPUT_DIR/
cp -r $TMPDIR/$ZLIB/* $OUTPUT_DIR/

mkdir -p $OUTPUT_DIR/share/xsl/docbook/
cp -r $TMPDIR/$DOCBOOK_XSL/* $OUTPUT_DIR/share/xsl/docbook/

mkdir -p $OUTPUT_DIR/share/xml/docbook/$DOCBOOK_XML_VER/
unzip -q -d $OUTPUT_DIR/share/xml/docbook/$DOCBOOK_XML_VER/ $FILES/docbook-xml-$DOCBOOK_XML_VER.zip

sed "s/DIRNAME/$OUTPUT_NAME/g" docs/user-config.jam > $OUTPUT_DIR/user-config.jam
sed "s/DIRNAME/$OUTPUT_NAME/g" docs/README.txt > $OUTPUT_DIR/README.txt

cd $TMPDIR
zip -q -r $OUTPUT_NAME.zip $OUTPUT_NAME
mv $TMPDIR/$OUTPUT_NAME.zip $DESTDIR
cd -

rm -r $TMPDIR
