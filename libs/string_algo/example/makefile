BOOST_INCLUDE=d:/boost

all: main.exe

main.exe: \
	main.cpp \
	../../../boost/string_algo/string_trim.hpp \
	../../../boost/string_algo/string_conv.hpp \
	../../../boost/string_algo/string_predicate.hpp \
	../../../boost/string_algo/string_funct.hpp \
	../../../boost/string_algo/string_find.hpp \
	../../../boost/string_algo/string_traits.hpp \
	../../../boost/string_algo/string_replace.hpp \
	../../../boost/string_algo/string_erase.hpp \
	../../../boost/string_algo.hpp

	g++ -I $(BOOST_INCLUDE) -I ../../.. main.cpp -o main.exe
