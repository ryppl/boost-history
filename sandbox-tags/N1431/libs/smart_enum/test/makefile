BOOST_ROOT     ?= /home/simons/projects/boost-current

CXX		= g++
CPPFLAGS	= -I$(BOOST_ROOT)
CXXFLAGS	= -Wall -pedantic -Wno-long-long -O3
LDFLAGS		= -L$(BOOST_ROOT)/libs/test/build/bin/libunit_test_framework.a/gcc/debug/runtime-link-dynamic/threading-single
LIBS		= -lunit_test_framework

SOURCES		= test_general.cpp
		  #test_bounded_enum.cpp	\
		  #test_lambda_enum.cpp		\
		  #test_sequential_enum.cpp	\
	          #test_wrapped_enum.cpp

% : %.cpp
	@echo Compiling $<
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

all:		$(SOURCES:.cpp=)

clean:
	rm -f $(SOURCES:.cpp=)

