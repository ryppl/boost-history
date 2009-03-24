#!/bin/sh -e

ICONV=iconv-1.9.2.win32
LIBXML=libxml2-2.7.3.win32
LIBXSLT=libxslt-1.1.24.win32
ZLIB=zlib-1.2.3.win32
DOCBOOK_XSL=docbook-xsl-1.74.3
DOCBOOK_DTD_VER=4.2

OUTPUT_NAME=win32-doc-tools-0.1
FILES=$(pwd)/files
TMPDIR=$(pwd)/tmp
DESTDIR=$(pwd)

SOURCEFORGE_MIRROR=http://dl.sourceforge.net

################################################################################
# Download files, and check for quickbook

# Could just use 'wget -nc'....

function download {
	FILENAME=$(basename $1)

	if test -f $FILES/$FILENAME; then
		echo "Using existing $FILENAME"
	else
		echo "Downloading $FILENAME"
		curl -o $FILES/$FILENAME $1
	fi
}

if test ! -f $FILES/quickbook.exe; then
	echo "files/quickbook.exe not found."
	exit 1
fi

download http://www.zlatkovic.com/pub/libxml/$ICONV.zip
download http://www.zlatkovic.com/pub/libxml/$LIBXSLT.zip
download http://www.zlatkovic.com/pub/libxml/$LIBXML.zip
download http://www.zlatkovic.com/pub/libxml/$ZLIB.zip
download $SOURCEFORGE_MIRROR/sourceforge/docbook/$DOCBOOK_XSL.zip
download http://www.oasis-open.org/docbook/xml/$DOCBOOK_DTD_VER/docbook-xml-$DOCBOOK_DTD_VER.zip

################################################################################

OUTPUT_DIR=$TMPDIR/$OUTPUT_NAME

# Remove old temporary directory
if test -d $TMPDIR; then
	rm -r $TMPDIR
fi

# Install quickbook
mkdir -p $OUTPUT_DIR/bin
cp $FILES/quickbook.exe $OUTPUT_DIR/bin

# Install zipfiles
function install_zipfile {
	NAME=$1
	TGT_DIR=$2
	
	unzip -q -d $TMPDIR $FILES/$NAME.zip
	
	# Rename readme.txt files so they don't clash
	if test -f $TMPDIR/$NAME/readme.txt; then
		mv $TMPDIR/$NAME/readme.txt $TGT_DIR/README-$NAME.txt
	fi

	mkdir -p $TGT_DIR
	cp -pR $TMPDIR/$NAME/* $TGT_DIR/
	
	rm -r $TMPDIR/$NAME/
}

install_zipfile $ICONV $OUTPUT_DIR
install_zipfile $LIBXSLT $OUTPUT_DIR
install_zipfile $LIBXML $OUTPUT_DIR
install_zipfile $ZLIB $OUTPUT_DIR
install_zipfile $DOCBOOK_XSL $OUTPUT_DIR/share/xsl/docbook/

# Deal with the DTD zipfile separately as it doesn't unzip to a single directory.

mkdir -p $OUTPUT_DIR/share/xml/docbook/$DOCBOOK_DTD_VER/
unzip -q -d $OUTPUT_DIR/share/xml/docbook/$DOCBOOK_DTD_VER/ $FILES/docbook-xml-$DOCBOOK_DTD_VER.zip

################################################################################

sed "s/DIRNAME/$OUTPUT_NAME/g" docs/user-config.jam > $OUTPUT_DIR/user-config.jam
sed "s/DIRNAME/$OUTPUT_NAME/g" docs/README.txt > $OUTPUT_DIR/README.txt

cd $TMPDIR
7za a $OUTPUT_NAME.7z $OUTPUT_NAME
mv $TMPDIR/$OUTPUT_NAME.7z $DESTDIR
7za a $OUTPUT_NAME.zip $OUTPUT_NAME
mv $TMPDIR/$OUTPUT_NAME.zip $DESTDIR
cd -

rm -r $TMPDIR
