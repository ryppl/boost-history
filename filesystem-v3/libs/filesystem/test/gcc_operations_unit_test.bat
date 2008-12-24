@echo off
g++ -DBOOST_ALL_NO_LIB -DBOOST_FILEYSTEM_INCLUDE_IOSTREAM -I../../.. -I%BOOST_TRUNK% operations_unit_test.cpp ../src/operations.cpp ../src/path.cpp ../src/portability.cpp ../src/utf8_codecvt_facet.cpp %BOOST_TRUNK%/libs/system/src/error_code.cpp 2>log
firefox log

