CXX		= g++
CPPFLAGS	=
CXXFLAGS	= -Wall -pedantic -Wno-long-long -O3
LDFLAGS		=
LIBS		= -lboost_unit_test_framework

SOURCES		= test_general.cpp		\
		  test_bounded_enum.cpp		\
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
