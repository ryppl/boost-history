set TOOLS=gcc vc7
set BUILD=debug release
bjam --dump-tests test >bjam.log 2>&1
