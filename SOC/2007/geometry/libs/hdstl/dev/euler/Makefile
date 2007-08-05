CXX=g++
CXXFLAGS=-pedantic -W -Wall -I.

LD=g++

TARGET=dbg_exc_mt

PACKAGE_OBJECTS=base_functions.o 

PACKAGE_LIBRARY=libhalfedge_ds.a

TEST_DRIVERS=base_functions.t 

.SUFFIXES:
.SUFFIXES: ${TARGET}.o .o .cpp .hpp

all: package_library test


### BUILDING PACKAGE LIBRARY AND DEPENDENCIES

package_library: ${PACKAGE_OBJECTS}
	ar -rv ${PACKAGE_LIBRARY} ${PACKAGE_OBJECTS}

%.o: %.cpp %.h
	${CXX} ${CXXFLAGS} ${CXXINCLUDES} -c -o $*.o $<


base_functions.o:  


### BUILDING TEST DRIVERS AND DEPENDENCIES OF TEST DRIVER

test: ${TEST_DRIVERS}
	for file in ${TEST_DRIVERS}; do echo "******* TESTING $$file *********"; ./$$file; done

%.t: %.t.o %.o
	${LD} ${LDFLAGS} -o $*.t $^

base_functions.t.o: 

%.t.o: %.t.cpp %.hpp
	${CXX}  ${CXXFLAGS} ${CXXINCLUDES} -c -o $*.t.o $<

base_functions.t:  

## UTILITIES

clean: realclean testclean

testclean:
	- rm *.t.o
	- rm *.t

realclean: 
	- rm *.o
	- rm ${PACKAGE_LIBRARY}

