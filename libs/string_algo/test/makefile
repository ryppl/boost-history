BOOST_INCLUDE=D:/boost

HEADERS= \
	test_main.cpp \
	trim_test.cpp \
	case_conv_test.cpp \
	string_predicate_test.cpp \
	../../../boost/string_algo/trim.hpp \
	../../../boost/string_algo/case_conv.hpp \
	../../../boost/string_algo/string_predicate.hpp \
	../../../boost/string_algo.hpp

SOURCES= \
	test_main.cpp \
	trim_test.cpp \
	case_conv_test.cpp \
    string_predicate_test.cpp

all: test.exe

test.exe: $(HEADERS) $(SOURCES)
	g++ -I $(BOOST_INCLUDE) -I ../../.. $(SOURCES) -o test.exe
