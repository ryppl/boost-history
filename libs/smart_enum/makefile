#
# Build smart_enum<> test programs.
#

CXX		= g++
CPPFLAGS        = -I/home/simons/projects/boost-current -I../..
CXXFLAGS        = -Wall -ansi -pedantic -Wno-long-long
LDFLAGS	        =
LIBS		=

TESTS		=  test_bounded_enum.exe test_lambda_enum.exe \
		   test_sequential_enum.exe test_wrapped_enum.exe

.SUFFIXES:
.SUFFIXES:	.cpp .exe

.cpp.exe:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $< -o $@

.PHONY:		clean

all::		$(TESTS)

check::		$(TESTS)
	@for n in $(TESTS); do echo -en "$${n%%.exe}:\t"; ./$$n; done

clean::
	@rm -f $(TESTS)
	@echo All dependent files have been removed.

$(TESTS):	../../boost/smart_enum.hpp
