#
# Build smart_enum<> test programs.
#

CXX		= g++
CPPFLAGS        = -I/home/simons/projects/boost-current -I../..
CXXFLAGS        = -O3 -Wall -ansi -pedantic -Wno-long-long
LDFLAGS	        =
LIBS		=

TESTS		= test_sequential_enum.exe test_wrapped_enum.exe \
		  test_bounded_enum.exe test_lambda_enum.exe

.SUFFIXES:
.SUFFIXES:	.cpp .exe

.cpp.exe:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $< -o $@

.PHONY:		all check clean dist

all::		$(TESTS)

check::		$(TESTS)
	@for n in $(TESTS); do echo -en "$${n%%.exe}:\t"; ./$$n; done

clean::
	@rm -f $(TESTS)
	@echo All dependent files have been removed.

dist::
	@rm -rf /tmp/smart-enum-src
	@mkdir -p /tmp/smart-enum-src/boost /tmp/smart-enum-src/libs/smart_enum
	@install -c -m 444 ../../boost.css ../../c++boost.gif /tmp/smart-enum-src/
	@install -c -m 444 ../../boost/smart_enum.hpp /tmp/smart-enum-src/boost/
	@install -c -m 644 makefile *.htm *.cpp *.gif /tmp/smart-enum-src/libs/smart_enum/
	@echo >/tmp/smart-enum-src/README "Example programs and documentation are in libs/smart_enum/."
	@(cd /tmp && tar cfvz smart-enum-src.tar.gz smart-enum-src)
	@rm -rf /tmp/smart-enum-src
	@echo Built release archive /tmp/smart-enum-src.tar.gz

$(TESTS):	../../boost/smart_enum.hpp
