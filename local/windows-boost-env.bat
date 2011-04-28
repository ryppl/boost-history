
@ECHO OFF

set HOMEDRIVE=e:
set HOMEPATH=/

set BOOST_ROOT=e:\sandbox\boost-trunk.windows
set BOOST_BUILD_PATH=%BOOST_ROOT%\tools\build\v2
set PATH=%PATH%;C:\PROGRA~1\boost\xml\bin;%BOOST_BUILD_PATH%\engine\bin.ntx86

ECHO HOME=%HOMEDRIVE%%HOMEPATH%
ECHO BOOST_ROOT=%BOOST_ROOT%
ECHO BOOST_BUILD_PATH=%BOOST_BUILD_PATH%
ECHO.
ECHO Make sure your "%HOMEDRIVE%%HOMEPATH%user-config.jam" exists and it is correct.

