BOOST_INCLUDE=D:/boost

HEADERS= \
	../../../boost/string_algo/trim.hpp \
	../../../boost/string_algo/conv.hpp \
	../../../boost/string_algo/predicate.hpp \
	../../../boost/string_algo/find.hpp \
	../../../boost/string_algo/find_impl.hpp \
	../../../boost/string_algo/traits.hpp \
	../../../boost/string_algo/replace.hpp \
	../../../boost/string_algo/replace_impl.hpp \
	../../../boost/string_algo/erase.hpp \
	../../../boost/string_algo/detail/case_conv.hpp \
	../../../boost/string_algo/detail/find.hpp \
	../../../boost/string_algo/detail/replace.hpp \
	../../../boost/string_algo/detail/trim.hpp \
	../../../boost/string_algo/detail/util.hpp \
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
