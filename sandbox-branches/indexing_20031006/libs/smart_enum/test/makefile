BOOST_ROOT     ?= /home/simons/projects/boost-current

CXX		= g++
CPPFLAGS	= -I$(BOOST_ROOT)
CXXFLAGS	= -Wall -pedantic -Wno-long-long -O3
LDFLAGS		= -L$(BOOST_ROOT)/libs/test/build/bin/libboost_unit_test_framework.a/gcc/debug/runtime-link-dynamic/threading-single
LIBS		= -lboost_unit_test_framework

SOURCES		= test_general.cpp		\
		  #test_bounded_enum.cp		\
		  #test_lambda_enum.cpp		\
		  #test_sequential_enum.cpp	\
	          #test_wrapped_enum.cpp

.PHONY: all clean dist import

% : %.cpp
	@echo Compiling $<
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

all:		$(SOURCES:.cpp=)

clean:
	rm -f $(SOURCES:.cpp=)

dist:
	rm -rf /tmp/smart-enum/boost
	install -d /tmp/smart-enum/boost
	install -d /tmp/smart-enum/libs/smart_enum/{doc,test}
	install -c -m 644 ../../../boost/smart_enum.hpp /tmp/smart-enum/boost/
	install -c -m 644 ../doc/{*.htm,*.png} /tmp/smart-enum/libs/smart_enum/doc/
	install -c -m 644 *.cpp makefile /tmp/smart-enum/libs/smart_enum/test/
	rm -f /tmp/smart-enum/smart-enum-src.tar.gz
	(cd /tmp && tar cfvz smart-enum-src.tar.gz smart-enum)
	install -m 644 /tmp/smart-enum-src.tar.gz /tmp/smart-enum/
	rm -f /tmp/smart-enum-src.tar.gz

import:		dist
	(cd /tmp/smart-enum && cvs import -m "Imported from boost-sandbox." homepage/smart-enum smart-enum smart-enum-publish)
