CXX             := g++
CXXFLAGS        := -ggdb3
INCPATH         := -I../../.. -I../../../../../include
LINK            := g++
LFLAGS          := -L../../../../../lib
RM              := rm
HEADERS         := $(wildcard *.h)
SOURCES         := $(wildcard *.cpp)
OBJECTS         := $(patsubst %.cpp,%.o,$(SOURCES))

.SUFFIXES: .cpp

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.PHONY : all depend clean


all : pack_ptr_test1 pack_ptr_test2 pack_ptr_test3 local_pool_test1 local_pool_test2 t100_test1

pack_ptr_test1: pack_ptr_test1.o
	$(LINK) $(LFLAGS) -o $@ $^

pack_ptr_test2: pack_ptr_test2.o
	$(LINK) $(LFLAGS) -o $@ $^

pack_ptr_test3: pack_ptr_test3.o
	$(LINK) $(LFLAGS) -o $@ $^ -lboost_unit_test_framework

local_pool_test1: local_pool_test1.o
	$(LINK) $(LFLAGS) -o $@ $^

local_pool_test2: local_pool_test2.o
	$(LINK) $(LFLAGS) -o $@ $^

t100_test1: t100_test1.o
	$(LINK) $(LFLAGS) -o $@ $^ -lboost_regex


Makefile.dep: $(SOURCES)
	$(CXX) ${INCPATH} -MM $^ > $@

clean:
	$(RM) pack_ptr_test1 pack_ptr_test2 pack_ptr_test3 local_pool_test1 local_pool_test2 t100_test1
	$(RM) $(OBJECTS)
	$(RM) *~ core

-include Makefile.dep
