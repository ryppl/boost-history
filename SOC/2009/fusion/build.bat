@echo off
rem Copyright Christopher Schmidt 2009.
rem Distributed under the Boost Software License, Version 1.0.
rem (See accompanying file LICENSE_1_0.txt or copy at
rem http://www.boost.org/LICENSE_1_0.txt)

rem (re-)set environment variables
rem bjam does not like backslahes!
set BOOST_ROOT=d:/projekte/libs/boost
set BOOST_INCLUDES_PATH=%BOOST_ROOT%/include/boost-1_39

rem call toolset specific procedure
if "%1" == "conceptgcc" (
    call :conceptgcc
) else (
    if "%1" == "gcc" (
        call :gcc
    ) else (
        if "%1" == "msvc" (
            call :msvc
        ) else (
            if "%1" == "" (
                call :conceptgcc
            ) else (
                echo Unknown toolset '%1'
                goto :eof
            )
        )
    )
)

rem compile...
%BOOST_ROOT%\bjam --without-python --toolset=%TOOLSET% %2 %3 %4 %5 %6 %7 %8 %9
goto :eof

:conceptgcc
rem ConceptGCC needs a cygwin path
set BOOST_INCLUDES_PATH=/cygdrive/d/projekte/libs/boost/include/boost-1_39

set PATH=C:\cygwin\opt\conceptgcc-4.3.0-alpha-7\bin;C:\cygwin\bin;%PATH%
set TOOLSET=gcc-conceptgcc
goto :eof

:gcc
set PATH=C:\MinGW\bin;%PATH%
set TOOLSET=gcc
goto :eof

:msvc
call "%VS90COMNTOOLS%..\..\VC\vcvarsall" x86
set TOOLSET=msvc
goto :eof
