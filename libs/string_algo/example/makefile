BOOST_INCLUDE=d:/boost

all: main.exe

main.exe: \
	main.cpp \
	../../../boost/string_algo/trim.hpp \
	../../../boost/string_algo/conv.hpp \
	../../../boost/string_algo/predicate.hpp \
	../../../boost/string_algo/find.hpp \
	../../../boost/string_algo/find_impl.hpp \
	../../../boost/string_algo/traits.hpp \
	../../../boost/string_algo/replace.hpp \
	../../../boost/string_algo/replace_impl.hpp \
	../../../boost/string_algo/erase.hpp \
	../../../boost/string_algo/detail/container.hpp \
	../../../boost/string_algo/detail/case_conv.hpp \
	../../../boost/string_algo/detail/find.hpp \
	../../../boost/string_algo/detail/replace.hpp \
	../../../boost/string_algo/detail/trim.hpp \
	../../../boost/string_algo/detail/util.hpp \
	../../../boost/string_algo.hpp

	g++ -I $(BOOST_INCLUDE) -I ../../.. main.cpp -o main.exe
