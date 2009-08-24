@echo off
rem ===========================================================================
rem    Copyright (c) 2009 Christopher Schmidt
rem
rem   Use, modification and distribution is subject to the Boost Software
rem   License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
rem   http://www.boost.org/LICENSE_1_0.txt)
rem ===========================================================================

rem (re-)set environment variables
set BOOST_ROOT=d:/projekte/libs/boost
set BOOST_INCLUDES_PATH=%BOOST_ROOT%/include/boost-1_40

rem call toolset specific procedure
if "%1" == "gcc4" (
    call :gcc4
) else (
    if "%1" == "vc9" (
        call :vc9
    ) else (
        if "%1" == "gcc3" (
            call :gcc3
        ) else (
            if "%1" == "" (
                call :gcc4
            ) else (
                echo Unknown toolset '%1'
                goto :eof
            )
        )
    )
)

rem compile...
%BOOST_ROOT%\bjam -sHOME="z:\projekte\cl_link_frontend\util\boost" --toolset=%TOOLSET% %2 %3 %4 %5 %6 %7 %8 %9
goto :eof

:gcc3
set PATH=C:\Program Files (x86)\MinGWOld\bin;%PATH%
set TOOLSET=gcc
goto :eof

:gcc4
set PATH=C:\Program Files (x86)\MinGW\bin;%PATH%
set TOOLSET=gcc
goto :eof

:vc9
call "%VS90COMNTOOLS%..\..\VC\vcvarsall" x86
set TOOLSET=msvc
goto :eof
