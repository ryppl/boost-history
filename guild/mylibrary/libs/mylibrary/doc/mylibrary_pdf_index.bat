echo off
rem quickbook doxgen auto-index docs template mylibrary_pdf_index.bat
rem echo mylibrary_pdf_index_%date%_%time:~0,2%_%time:~3,2%.log
i:
cd \boost-sandbox\guild\mylibrary\libs\mylibrary\doc
bjam -a pdf --enable-index > mylibrary_pdf_index_%date%_%time:~0,2%%time:~3,2%.log
if not ERRORLEVEL 0 (echo Errorlevel is %ERRORLEVEL%) else (echo OK)
pause
