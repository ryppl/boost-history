
# Copyright (C) 2009-2011 Lorenzo Caminiti
# Use, modification, and distribution is subject to the
# Boost Software License, Version 1.0
# (see accompanying file LICENSE_1_0.txt or a copy at
# http://www.boost.org/LICENSE_1_0.txt).

# Source this file `$ source ...`.

# Setup environment to build examples and documentation using Boost.BJam.

echo '
WARNING: This script is NOT smart -- it assumes all Boost installation paths. Edit this script if these paths do not match your Boost installation!

Before running this script you must have downloaded and compiled Boost.Build (to build the examples), Boost.QuickBook (to build the documentation), and installed Boost.QuickBook related tools (DocBook, XSL, etc) -- see the Boost online documentation.
However, you do not strictly need to do this because you can use any compiler to build the examples (just add this headers-only library to your #include path) and you do not need to re-build the documentation.
'

export BOOST_ROOT="C:\PROGRA~1\boost\boost_1_45_0"
export BOOST=$BOOST_ROOT
export BOOST_BUILD_PATH="$BOOST_ROOT\tools\build\v2"
export PATH="$PATH:/cygdrive/c/PROGRA~1/boost/boost_1_45_0/tools/build/v2/engine/src/bin.ntx86"

user_config_jam="$HOME/user-config.jam"
if [ -e $user_config_jam ]; then
    echo "Assumed existing $user_config_jam has correct paths for Boost.QuickBook tools, change this file if that is not the case."
else
    echo '
import os ;
local BOOST_ROOT = [ os.environ BOOST_ROOT ] ;

using msvc ; # Toolset.

# Boost.QuickBook (and related tools).
using quickbook : $(BOOST_ROOT)"/bin.v2/tools/quickbook/src/msvc-8.0/debug/link-static/threading-multi/quickbook.exe" : ;
using xsltproc : "C:/Users/example/Documents/boost/xml/bin/xsltproc.exe" ;
using boostbook : "C:/Users/example/Documents/boost/xml/docbook-xsl" : "C:/Users/example/Documents/boost/xml/docbook-xml" ;
using doxygen : "C:/Program Files/doxygen/bin/doxygen.exe" ;
' > $user_config_jam
    echo "Created $user_config_jam (assumed Boost.QuickBook tool paths)."
fi

