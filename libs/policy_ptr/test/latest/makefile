CFLAGS = -Wall -ggdb
IDIR = ..
SDIR = /usr/local/boost-1.30.0/libs/test/src
OBJS = execution_monitor.o test_tools.o unit_test_log.o \
    unit_test_main.o unit_test_monitor.o unit_test_parameters.o \
    unit_test_result.o unit_test_suite.o

smart_ptr_test: smart_ptr_test.o $(OBJS)
	g++ -o smart_ptr_test smart_ptr_test.o $(OBJS)

smart_ptr_test.o: $(IDIR)/smart_ptr.hpp \
        $(IDIR)/optimally_inherit.hpp smart_ptr_test.cpp
	g++ -c $(CFLAGS) smart_ptr_test.cpp

execution_monitor.o: $(SDIR)/execution_monitor.cpp
	g++ -c $(CFLAGS) $(SDIR)/execution_monitor.cpp

test_tools.o: $(SDIR)/test_tools.cpp
	g++ -c $(CFLAGS) $(SDIR)/test_tools.cpp

unit_test_log.o: $(SDIR)/unit_test_log.cpp
	g++ -c $(CFLAGS) $(SDIR)/unit_test_log.cpp

unit_test_main.o: $(SDIR)/unit_test_main.cpp
	g++ -c $(CFLAGS) $(SDIR)/unit_test_main.cpp

unit_test_monitor.o: $(SDIR)/unit_test_monitor.cpp
	g++ -c $(CFLAGS) $(SDIR)/unit_test_monitor.cpp

unit_test_parameters.o: $(SDIR)/unit_test_parameters.cpp
	g++ -c $(CFLAGS) $(SDIR)/unit_test_parameters.cpp

unit_test_result.o: $(SDIR)/unit_test_result.cpp
	g++ -c $(CFLAGS) $(SDIR)/unit_test_result.cpp

unit_test_suite.o: $(SDIR)/unit_test_suite.cpp
	g++ -c $(CFLAGS) $(SDIR)/unit_test_suite.cpp

no_check: $(OBJS) $(IDIR)/smart_ptr.hpp $(IDIR)/optimally_inherit.hpp \
        no_check.cpp
	g++ -o no_check $(OBJS) no_check.cpp

assert_check: $(OBJS) $(IDIR)/smart_ptr.hpp $(IDIR)/optimally_inherit.hpp \
        assert_check.cpp
	g++ -o assert_check $(OBJS) assert_check.cpp

reject_null_static: $(OBJS) $(IDIR)/smart_ptr.hpp \
        $(IDIR)/optimally_inherit.hpp reject_null_static.cpp
	g++ -o reject_null_static $(OBJS) reject_null_static.cpp

reject_null_strict: $(OBJS) $(IDIR)/smart_ptr.hpp \
        $(IDIR)/optimally_inherit.hpp reject_null_strict.cpp
	g++ -o reject_null_strict $(OBJS) reject_null_strict.cpp

all: smart_ptr_test no_check assert_check

shared_ptr_basic:  $(IDIR)/smart_ptr.hpp $(IDIR)/optimally_inherit.hpp \
        $(IDIR)/policy/boost.hpp shared_ptr_basic_test.cpp
	g++ -o shared_ptr_basic shared_ptr_basic_test.cpp

shared_ptr_main:  $(IDIR)/smart_ptr.hpp $(IDIR)/optimally_inherit.hpp \
        $(IDIR)/policy/boost.hpp shared_ptr_test.cpp
	g++ -o shared_ptr_main shared_ptr_test.cpp
