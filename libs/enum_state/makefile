#
# Build checked_enum<> test program.
#

CXX		= g++

OPTIMFLAGS     += -O3
WARNFLAGS      += -Wall -ansi -pedantic -Wno-long-long
CPPFLAGS       += -I/home/simons/projects/boost-current -I/home/simons/projects/boost-sandbox
CXXFLAGS       += -frepo
LDFLAGS	       += -frepo

OBJS	       += test.o
LIBS	       +=

.SUFFIXES:	.o .cpp
.PHONY:		clean distclean realclean depend

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNFLAGS) $(OPTIMFLAGS) -c $< -o $@

all::		test

test:	$(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

check::		test
	.\/test

realclean distclean clean::
	@find . -name '*.rpo' -exec rm -f {} \;
	@find . -name '*.a'   -exec rm -f {} \;
	@find . -name '*.o'   -exec rm -f {} \;
	@find . -name '*.so'  -exec rm -f {} \;
	@rm -f test
	@echo All dependent files have been removed.

depend::
	makedepend -Y $(CPPFLAGS) -fmakefile '-s# Dependencies' *.cpp 2>/dev/null
	@sed <makefile >makefile.bak -e 's/\.\///g'
	@mv makefile.bak makefile

# Dependencies

test.o: /home/simons/projects/boost-current/boost/test/test_tools.hpp
test.o: /home/simons/projects/boost-current/boost/test/cpp_main.cpp
test.o: /home/simons/projects/boost-current/boost/detail/catch_exceptions.hpp
test.o: /home/simons/projects/boost-current/boost/cstdlib.hpp
test.o: /home/simons/projects/boost-current/boost/test/test_main.cpp
test.o: /home/simons/projects/boost-current/boost/static_assert.hpp
test.o: /home/simons/projects/boost-current/boost/config.hpp
test.o: /home/simons/projects/boost-current/boost/config/user.hpp
test.o: /home/simons/projects/boost-current/boost/config/select_compiler_config.hpp
test.o: /home/simons/projects/boost-current/boost/config/compiler/gcc.hpp
test.o: /home/simons/projects/boost-current/boost/config/select_stdlib_config.hpp
test.o: /home/simons/projects/boost-current/boost/config/select_platform_config.hpp
test.o: /home/simons/projects/boost-current/boost/config/suffix.hpp
