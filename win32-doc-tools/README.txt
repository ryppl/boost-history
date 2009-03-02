Download and put in the files directory:

* from http://www.zlatkovic.com/pub/libxml/:
    * iconv
    * libxml2
    * libxslt
    * zlib
* The latest docbool xsl stylesheets from
  http://sourceforge.net/project/showfiles.php?group_id=21935&package_id=16608
* Docbook XML 4.2 from: http://www.oasis-open.org/docbook/xml/4.2/docbook-xml-4.2.zip

Update the variables at the beginning of build.sh to match the versions you downloaded.

Build quickbook using 'bjam dist-bin' in '$BOOST_ROOT/tools/quickbook/'. And move
'$BOOST_ROOT/dist/bin/quickbook.exe' to the files directory.

Change to this directory and run 'build.sh'.