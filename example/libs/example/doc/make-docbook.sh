#!/bin/sh

# Quick example to show how docbook can be manually generated from doxygen.
#
# Requires BOOST_ROOT to be set to the locations where boost is installed.
# Also requires doxygen and xsltproc to be in your path.

BUILD_DIR=build

mkdir -p $BUILD_DIR/doxygen

# 1: Use doxygen to generate its xml based documentation:

cat > $BUILD_DIR/doxygen/example.doxyfile <<EOF
ENABLE_PREPROCESSING = YES
EXPAND_ONLY_PREDEF = YES
EXTRACT_ALL = NO
EXTRACT_PRIVATE = NO
GENERATE_HTML = NO
GENERATE_XML = YES
GENERATE_LATEX = NO
HIDE_UNDOC_MEMBERS = YES
MACRO_EXPANSION = YES
XML_OUTPUT = doxygen-xml
OUTPUT_DIRECTORY = $BUILD_DIR/
INPUT=../../../boost/example/example.hpp
EOF

doxygen $BUILD_DIR/doxygen/example.doxyfile

DOXYGEN_OUTPUT_DIR=$BUILD_DIR/doxygen-xml

# 2: Set up our xml catalog:

cat > $BUILD_DIR/catalog.xml <<EOF
<?xml version="1.0"?>
<!DOCTYPE catalog 
  PUBLIC "-//OASIS/DTD Entity Resolution XML Catalog V1.0//EN"
  "http://www.oasis-open.org/committees/entity/release/1.0/catalog.dtd">
<catalog xmlns="urn:oasis:names:tc:entity:xmlns:xml:catalog">
  <rewriteURI uriStartString="http://www.boost.org/tools/boostbook/dtd/"
    rewritePrefix="file://$BOOST_ROOT/tools/boostbook/dtd/"/>
</catalog>
EOF

export XML_CATALOG_FILES=$(pwd)/$BUILD_DIR/catalog.xml

# 3: Collect the doxygen output into a single file

xsltproc --stringparam doxygen.xml.path $DOXYGEN_OUTPUT_DIR \
    -o $BUILD_DIR/example.doxygen \
    $BOOST_ROOT/tools/boostbook/xsl/doxygen/collect.xsl \
    $DOXYGEN_OUTPUT_DIR/index.xml

# 4: Convert the doxygen xml to boostbook

xsltproc -o $BUILD_DIR/example.boostbook \
    $BOOST_ROOT/tools/boostbook/xsl/doxygen/doxygen2boostbook.xsl \
    $BUILD_DIR/example.doxygen

# 5: Convert the boostbook to docbook

xsltproc -o example.docbook \
    $BOOST_ROOT/tools/boostbook/xsl/docbook.xsl \
    $BUILD_DIR/example.boostbook
