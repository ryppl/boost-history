@echo off
rem  Create Boost Filesystem Version 3 distribution for Windows

rem  Copyright 2009 Beman Dawes
rem  Distributed under the Boost Software License, Version 1.0.
rem  See http://www.boost.org/LICENSE_1_0.txt

echo Create Boost Filesystem Version 3 distribution for Windows
if {%1}=={} goto usage

echo Prepare temporary directory %TEMP%\%1...
pushd %TEMP%
rmdir /S /Q %1 2>nul
del %1.zip 2>nul

echo Exporte https://svn.boost.org/svn/boost/sandbox/filesystem-v3 to %TEMP%\%1...
svn export --non-interactive --trust-server-cert --native-eol CRLF ^
 https://svn.boost.org/svn/boost/sandbox/filesystem-v3 %1

echo Creating %TEMP%\%1.zip...
zip -r -q %1.zip %1

echo Finish up...
popd
dir %TEMP%\%1.zip
goto done

:usage
echo Usage: create_distro distro-name
echo Will create %TEMP%\distro-name.zip
:done
