BOOST_INCLUDE=D:/boost

all: test.exe

test.exe: \
	test_main.cpp \
	trim_test.cpp \
	case_conv_test.cpp \
	../../../boost/string_algo/trim.hpp \
	../../../boost/string_algo/case_conv.hpp \
	../../../boost/string_algo.hpp
	
	g++ -I $(BOOST_INCLUDE) -I ../../.. test_main.cpp trim_test.cpp case_conv_test.cpp -o test.exe