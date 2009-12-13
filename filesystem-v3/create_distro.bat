@echo off
rem  Create Boost Filesystem Version 3 distribution for Windows

rem  Copyright 2009 Beman Dawes
rem  Distributed under the Boost Software License, Version 1.0.
rem  See http://www.boost.org/LICENSE_1_0.txt

echo Create Boost Filesystem Version 3 distribution for Windows
if {%1}=={} goto usage

pushd %TEMP%
echo Prepare temporary directory %TEMP%\%1 for Windows distro...
rmdir /S /Q %1 2>nul
del %1.zip 2>nul

echo Export https://svn.boost.org/svn/boost/sandbox/filesystem-v3 to %TEMP%\%1...
svn export --quiet --non-interactive --trust-server-cert --native-eol CRLF ^
 https://svn.boost.org/svn/boost/sandbox/filesystem-v3 %1

echo Create %TEMP%\%1.zip...
zip -r -q %1.zip %1

echo Prepare temporary directory %TEMP%\%1 for POSIX distro...
rmdir /S /Q %1 2>nul
del %1.tmp 2>nul
del %1.tar.gz 2>nul
del %1.tar.bz2 2>nul

echo Export https://svn.boost.org/svn/boost/sandbox/filesystem-v3 to %TEMP%\%1...
svn export --quiet --non-interactive --trust-server-cert --native-eol LF ^
 https://svn.boost.org/svn/boost/sandbox/filesystem-v3 %1

echo Create %TEMP%\%1.tar.gz...
tar cfz %1.tar.gz %1
echo Create %TEMP%\%1.tar.bz2...
gzip -d -c %1.tar.gz >%1.tmp
bzip2 <%1.tmp >%1.tar.bz2

echo Finish up...
popd
dir %TEMP%\%1.*
goto done

:usage
echo Usage: create_distro distro-name
echo Will create distro-name.zip, distro-name.tar.gz, distro-name.tar.bz2 in temp
:done
