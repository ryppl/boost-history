@echo off
rem  Install Boost Filesystem Version 3 distribution for Windows

rem  Copyright 2009 Beman Dawes
rem  Distributed under the Boost Software License, Version 1.0.
rem  See http://www.boost.org/LICENSE_1_0.txt

echo Install Boost Filesystem Version 3 distribution for Windows
if {%1}=={} goto usage
if {%2}=={} goto usage

if not exist %1 (
  echo error: %1 not found
  goto done
)

if not exist %2 (
  echo error: %2 not found
  goto done
)

if not exist %2\boost-build.jam (
  echo error: %2 does not contain boost-build.jam
  echo Is it really a Boost root directory?
  goto done
)

echo Delete prior version of Filesystem library...
del %2\boost\filesystem.hpp 2>nul
rmdir /S /Q %2\boost\filesystem 2>nul
rmdir /S /Q %2\libs\filesystem 2>nul

if not exist bjam.exe (
  echo Running bootstrap script...
  pushd %2
  call bootstrap >bootstrap.log 2>&1
  echo bootstrap complete - see %2\boostrap.log for details
  popd
)

echo Copy files from %1 to %2...
xcopy /s /q %1\* %2

echo Build libraries...
pushd %2
time /t
echo If other libraries have not been previously built, this step may take a
echo long time - 10 minutes on a fast machine, much longer on a slow machine.
.\bjam >bjam.log 2>&1
time /t
echo Build complete - see %2\bjam.log for details.
echo Libraries have been installed in %2\stage

echo Test Filesystem Version 3...
pushd libs\filesystem\test
time /t
..\..\..\bjam >bjam.log 2>&1
time /t
popd
find "...failed" libs\filesystem\test\bjam.log
echo Test complete - see %2\libs\filesystem\test\bjam.log for details.

echo Installation complete.
popd
goto done

:usage
echo Usage: install_distro distro-path boost-path
echo Installs the Filesystem V3 distribution file found at distro-path
echo in the Boost root found at boost-path 
:done
