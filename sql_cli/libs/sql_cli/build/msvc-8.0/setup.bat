REM CommandInterpreter: $(COMSPEC)
if "%BOOST_ROOT%"=="" set BOOST_ROOT="..\..\..\.."
pushd %BOOST_ROOT%\libs\signals\build
bjam --v2 toolset=msvc-8.0 variant=debug threading=multi
popd
