BOOST_INCLUDE=D:/boost

HEADERS= \
	../../../boost/string_algo/string_trim.hpp \
	../../../boost/string_algo/string_conv.hpp \
	../../../boost/string_algo/string_predicate.hpp \
	../../../boost/string_algo/string_funct.hpp \
	../../../boost/string_algo/string_substr.hpp \
	../../../boost/string_algo.hpp


SOURCES= \
	test_main.cpp \
	trim_test.cpp \
	conv_test.cpp \
    predicate_test.cpp \
	substr_test.cpp

all: test.exe

test.exe: $(HEADERS) $(SOURCES)
	g++ -I $(BOOST_INCLUDE) -I ../../.. $(SOURCES) -o test.exe
