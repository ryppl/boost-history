bash .\yyacc jamgram.y jamgramtab.h jamgram.yy
set VISUALC=c:\tools\msvc6\vc98
set JAM_TOOLSET=VISUALC
set YACC="bison -t -d -l -v --debug --yacc"
set YACCFILES=y.tab
rem set CFLAGS="/GZ /Zi /MLd -DNT"
rem set CCFLAGS="/GZ" "/Zi" "/MLd"
set LINKLIBS="%VISUALC%\lib\advapi32.lib %VISUALC%\lib\oldnames.lib c:\tools\msvc6\vc98\lib\gdi32.lib %VISUALC%\lib\user32.lib %VISUALC%\lib\kernel32.lib"
set LINKFLAGS="/DEBUG"
rm -rf bin.ntx86
mkdir bin.ntx86
nmake -fbuilds/win32-visualc.mk JAMBASE= BOOST_ROOT= BOOST_BUILD_PATH= CFLAGS="/GZ /Zi /MLd -DNT" CCFLAGS="/GZ /Zi /MLd" YACC="bison -t -d -l -v --debug --yacc" LINKLIBS="c:\tools\msvc6\vc98\lib\advapi32.lib c:\tools\msvc6\vc98\lib\oldnames.lib c:\tools\msvc6\vc98\lib\gdi32.lib c:\tools\msvc6\vc98\lib\user32.lib c:\tools\msvc6\vc98\lib\kernel32.lib" LINKFLAGS="/DEBUG" %*
