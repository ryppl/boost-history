@Echo off

REM Setup Variables

SET PATHTOINK="C:\Program Files\Inkscape\inkscape.exe"
SET STARTPATH=%CD%

IF NOT EXIST %PATHTOINK% GOTO NOINK

REM SET DUMPATH=%CD%\..

IF EXIST "%DUMPATH%\22x22" GOTO CHECK32x32
	mkdir "%DUMPATH%\22x22"
:CHECK32x32

IF EXIST "%DUMPATH%\32x32" GOTO CHECK44x44
	mkdir "%DUMPATH%\32x32"
:CHECK44x44

IF EXIST "%DUMPATH%\44x44" GOTO CHECK64x64
	mkdir "%DUMPATH%\44x44"
:CHECK64x64

IF EXIST "%DUMPATH%\64x64" GOTO ENDCHECK
	mkdir "%DUMPATH%\64x64"
:ENDCHECK


for /R ..\ %%i in (*.svg) do %PATHTOINK% -f "%%~fi" -e "%DUMPATH%\22x22\%%~ni.png" -w 22 -h 22
for /R ..\ %%i in (*.svg) do %PATHTOINK% -f "%%~fi" -e "%DUMPATH%\32x32\%%~ni.png" -w 32 -h 32
for /R ..\ %%i in (*.svg) do %PATHTOINK% -f "%%~fi" -e "%DUMPATH%\44x44\%%~ni.png" -w 44 -h 44
for /R ..\ %%i in (*.svg) do %PATHTOINK% -f "%%~fi" -e "%DUMPATH%\64x64\%%~ni.png" -w 64 -h 64


GOTO EXITBAT

:NOINK

ECHO Please edit this batch file with the location of Inkscape.
ECHO Exiting ...

GOTO EXITBAT


:EXITBAT


