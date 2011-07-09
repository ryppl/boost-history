#!/bin/sh -e

ICONV=iconv-1.9.2.win32
LIBXML=libxml2-2.7.8.win32
LIBXSLT=libxslt-1.1.26.win32
ZLIB=zlib-1.2.5.win32
DOCBOOK_XSL_VERSION=1.76.1
DOCBOOK_XSL=docbook-xsl-${DOCBOOK_XSL_VERSION}
DOCBOOK_DTD_VER=4.2

OUTPUT_NAME=win32-doc-tools-0.1.2
FILES=$(pwd)/files
TMPDIR=$(pwd)/tmp
DESTDIR=$(pwd)

SOURCEFORGE_DOWNLOAD=http://sourceforge.net/projects/docbook/files/

################################################################################
# Download files, and check for quickbook

# Could just use 'wget -nc'....

download() {
	FILENAME=$2

	if test -f $FILES/$FILENAME; then
		echo "Using existing $FILENAME"
	else
		echo "Downloading $FILENAME"
		curl -L -o $FILES/$FILENAME $1
	fi
}

if test ! -f $FILES/quickbook.exe; then
	echo "files/quickbook.exe not found."
	exit 1
fi

download http://www.zlatkovic.com/pub/libxml/$ICONV.zip $ICONV.zip
download http://www.zlatkovic.com/pub/libxml/$LIBXSLT.zip $LIBXSLT.zip
download http://www.zlatkovic.com/pub/libxml/$LIBXML.zip $LIBXML.zip
download http://www.zlatkovic.com/pub/libxml/$ZLIB.zip $ZLIB.zip
download $SOURCEFORGE_DOWNLOAD/docbook-xsl/$DOCBOOK_XSL_VERSION/$DOCBOOK_XSL.zip/download $DOCBOOK_XSL.zip
download http://www.oasis-open.org/docbook/xml/$DOCBOOK_DTD_VER/docbook-xml-$DOCBOOK_DTD_VER.zip docbook-xml-$DOCBOOK_DTD_VER.zip

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
install_zipfile() {
	NAME=$1
	TGT_DIR=$2
	
	unzip -q -d $TMPDIR $FILES/$NAME.zip

    if test -d $TMPDIR/$NAME; then
        DIRNAME=$TMPDIR/$NAME
    else
        DIRNAME=$(echo $TMPDIR/$NAME | sed s/\.win32$//)
    fi
	
	# Rename readme.txt files so they don't clash
	if test -f $DIRNAME/readme.txt; then
		mv $DIRNAME/readme.txt $TGT_DIR/README-$NAME.txt
	fi

	mkdir -p $TGT_DIR
	cp -pR $DIRNAME/* $TGT_DIR/
	
	rm -r $DIRNAME/
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
