@echo off
rem Copyright Christopher Schmidt 2009.
rem Distributed under the Boost Software License, Version 1.0.
rem (See accompanying file LICENSE_1_0.txt or copy at
rem http://www.boost.org/LICENSE_1_0.txt)

rem (re-)set environment variables
set BOOST_ROOT=d:\projekte\libs\boost
set BOOST_INCLUDES_PATH=D:\projekte\libs\boost\include\boost-1_38
set HOME=

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
z:\projekte\cl_link_frontend\tools\bjam --toolset=%TOOLSET% mini-fusion
goto :eof

:conceptgcc
rem set HOME to the current path so bjam uses our custom user-config.jam with the custom invocation commands for ConceptGCC
set HOME=.
rem Add the workaround folder to the include paths so our workaround stl include files are found
set CPLUS_INCLUDE_PATH=./workaround/conceptgcc:
rem ConceptGCC needs a cygwin path
set BOOST_INCLUDES_PATH=/cygdrive/d/projekte/libs/boost/include/boost-1_38

set PATH=C:\cygwin\opt\conceptgcc-4.3.0-alpha-7\bin;C:\cygwin\bin;%PATH%
set TOOLSET=gcc
goto :eof

:gcc
set PATH=C:\MinGW\bin;%PATH%
set TOOLSET=gcc
goto :eof

:msvc
call "%VS90COMNTOOLS%..\..\VC\vcvarsall" x86
set TOOLSET=msvc
goto :eof