@echo off
g++ -DBOOST_ALL_NO_LIB -I../../.. -I%BOOST_TRUNK% path_unit_test.cpp ../src/path.cpp ../src/portability.cpp ../src/utf8_codecvt_facet.cpp %BOOST_TRUNK%/libs/system/src/error_code.cpp 2>log
firefox log

