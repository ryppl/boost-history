CFLAGS = -Wall
IDIR = ../../../boost/
SDIR = ../../../../boost/libs/test/src
OBJS = smart_ptr_test.o execution_monitor.o test_tools.o unit_test_log.o \
    unit_test_main.o unit_test_monitor.o unit_test_parameters.o \
    unit_test_result.o unit_test_suite.o supplied_log_formatters.o

smart_ptr_test: $(OBJS)
	g++ $(OBJS) -o smart_ptr_test

smart_ptr_test.o: $(IDIR)/policy_ptr/smart_ptr.hpp smart_ptr_test.cpp
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

supplied_log_formatters.o: $(SDIR)/supplied_log_formatters.cpp
	g++ -c $(CFLAGS) $(SDIR)/supplied_log_formatters.cpp

tidy:
	rm -f *~

clean: tidy
	rm -f *.o
	rm -f smart_ptr_test
