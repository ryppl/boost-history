
@ECHO OFF

REM Copyright (C) 2009-2011 Lorenzo Caminiti
REM Use, modification, and distribution is subject to the
REM Boost Software License, Version 1.0
REM (see accompanying file LICENSE_1_0.txt or a copy at
REM http://www.boost.org/LICENSE_1_0.txt).

REM Setup environment to build examples and documentation using Boost.BJam.

ECHO.
ECHO WARNING: This script is NOT smart -- it assumes all Boost installation paths. Edit this script if these paths do not match your Boost installation.
ECHO.
ECHO Before running this script you must have downloaded and compiled Boost.Build (to build the examples), Boost.QuickBook (to build the documentation), and installed Boost.QuickBook related tools (DocBook, XSL, etc) -- see the Boost online documentation.
ECHO However, you do not strictly need to do this because you can use any compiler to build the examples (just add this headers-only library to your #include path) and you do not need to re-build the documentation.
ECHO.

REM set BOOST_ROOT=C:\PROGRA~1\boost\boost_1_46_1
set BOOST_ROOT=e:\sandbox\boost-trunk
set LIBPATH=%LIBPATH%;%BOOST_ROOT%\stage\lib
set BOOST=%BOOST_ROOT%
set BOOST_BUILD_PATH=%BOOST_ROOT%\tools\build\v2
set PATH=%PATH%;C:\PROGRA~1\boost\xml\bin;%BOOST_BUILD_PATH%\engine\src\bin.ntx86

set user_config_jam="%HOMEDRIVE%%HOMEPATH%/user-config.jam"
if exist %user_config_jam% (
    goto :configured_jam
) else (
    goto :configure_jam
)
goto :end

:configured_jam
ECHO Assumed existing %user_config_jam% has correct paths for Boost.QuickBook tools, change this file if that is not the case.
goto :end

:configure_jam
ECHO. > %user_config_jam%
ECHO import os ; >> %user_config_jam%
ECHO local BOOST_ROOT = [ os.environ BOOST_ROOT ] ; >> %user_config_jam%
ECHO. >> %user_config_jam%
ECHO using msvc ; # Toolset. >> %user_config_jam%
ECHO. >> %user_config_jam%
ECHO # Boost.QuickBook (and related tools). >> %user_config_jam%
ECHO using quickbook : $(BOOST_ROOT)"/bin.v2/tools/quickbook/src/msvc-8.0/release/link-static/threading-multi/quickbook.exe" : ; >> %user_config_jam%
ECHO using xsltproc : "C:/Program Files/boost/xml/bin/xsltproc.exe" ; >> %user_config_jam%
ECHO using boostbook : "C:/Program Files/boost/xml/docbook-xsl" : "C:/Program Files/boost/xml/docbook-xml" ; >> %user_config_jam%
ECHO using doxygen : "C:/Program Files/doxygen/bin/doxygen.exe" ; >> %user_config_jam%
ECHO Created %user_config_jam% (assumed Boost.QuickBook tool paths).
goto :end

:end

