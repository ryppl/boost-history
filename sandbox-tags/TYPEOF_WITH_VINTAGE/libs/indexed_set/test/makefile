BOOST           = /usr/include/boost_1_30_0
CXX             = g++
EXTRAFLAGS      = -pedantic -Wno-long-long -Wall 
INCLUDES        = -I$(BOOST) -I./../../..
CXXFLAGS        = $(INCLUDES) $(EXTRAFLAGS)

SOURCES = \
  test_basic.cpp\
  test_capacity.cpp\
  test_comparison.cpp\
  test_conv_iterators.cpp\
  test_copy_assignment.cpp\
  test_iterators.cpp\
  test_key_extractors.cpp\
  test_list_ops.cpp\
  test_modifiers.cpp\
  test_mpl_ops.cpp\
  test_projection.cpp\
  test_range.cpp\
  test_safe_mode.cpp\
  test_set_ops.cpp\
  test_special_set_ops.cpp\
  test_update.cpp\

MAINS           = ${SOURCES:.cpp=_main.cpp}

OBJECTS         = ${SOURCES:.cpp=.o} ${MAINS:.cpp=.o}

TARGETS         = ${SOURCES:.cpp=.exe}     

all: $(TARGETS)

%.exe: %.o %_main.o
	$(CXX) -o $@ $?

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:	
	/bin/rm -rf $(TARGETS) $(OBJECTS)

run:	
	test_basic.exe
	test_capacity.exe
	test_comparison.exe
	test_conv_iterators.exe
	test_copy_assignment.exe
	test_iterators.exe
	test_modifiers.exe
	test_projection.exe
	test_range.exe
	test_safe_mode.exe
	test_set_ops.exe
	test_special_set_ops.exe
	test_update.exe
