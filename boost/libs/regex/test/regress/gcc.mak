
# very basic makefile for regression tests
#
# g++ 2.95 and greater
#
CXX= g++ $(INCLUDES) -L../../../../stage/lib -I../../../../ -I./ $(CXXFLAGS) -L../../build/gcc $(LDFLAGS)
#
# sources to compile for each test:
#
SOURCES=basic_tests.cpp main.cpp test_deprecated.cpp ../../../test/src/cpp_main.cpp ../../../test/src/execution_monitor.cpp

total : gcc_regress
	export LD_LIBRARY_PATH="../../build/gcc:$LD_LIBRARY_PATH" && ./gcc_regress tests.txt

gcc_regress : $(SOURCES)
	$(CXX) -O2 -o gcc_regress $(SOURCES) ../../build/gcc/libboost_regex-gcc*.a $(LIBS)

debug : $(SOURCES)
	$(CXX) -g -o gcc_regress $(SOURCES) ../../build/gcc/libboost_regex-gcc-d*.a $(LIBS)
















































