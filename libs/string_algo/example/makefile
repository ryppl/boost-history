all: main.exe

main.exe: \
	main.cpp \
	../../../boost/string_algo/trim.hpp \
	../../../boost/string_algo/case_conv.hpp \
	../../../boost/string_algo.hpp

	g++ -I ../../.. main.cpp -o main.exe