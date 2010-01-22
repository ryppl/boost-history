rem Installs boost::date_time and boost::test so all examples and tests can be run
rem for the Interval Template Library (ITL). You do not need to run this script,
rem if you have a working installation of the boost libraries.

@echo off
if "%1"=="h"      goto help
if "%1"=="-h"     goto help
if "%1"=="help"   goto help
if "%1"=="-help" goto help
if "%1"=="--help" goto help

if     "%1"=="" set TOOL_SET=msvc
if not "%1"=="" set TOOL_SET=%1

bjam toolset=%TOOL_SET% --with-date_time --build-type=complete stage
bjam toolset=%TOOL_SET% --with-test --build-type=complete stage
goto end

:help
echo Usage: install_libs [toolset or -h --help]
echo Argument: -h --help : Show usage 
echo           toolset : The compiler 'toolset' will be used with bjam. 
echo                     If the first argument is omitted 'msvc' is used as toolset.
echo Installs boost::date_time and boost::test so all examples and tests can be run
echo for the Interval Template Library (ITL). You do not need to run this script,
echo if you have a working installation of the boost libraries.

:end

