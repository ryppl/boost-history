CXX=g++
CXXFLAGS=-pedantic -W -Wall  -I.
# CXXFLAGS=-pedantic -W -Wall -I${BOOST} -I.

LD=g++

TARGET=dbg_exc_mt

PACKAGE_OBJECTS=container_selectors.o

PACKAGE_LIBRARY=libhalfedge_ds.a

TEST_DRIVERS=container_selectors.t

.SUFFIXES:
.SUFFIXES: ${TARGET}.o .o .cpp .hpp

all: package_library test


### BUILDING PACKAGE LIBRARY AND DEPENDENCIES

package_library: ${PACKAGE_OBJECTS}
	ar -rv ${PACKAGE_LIBRARY} ${PACKAGE_OBJECTS}

%.o: %.cpp %.h
	${CXX} ${CXXFLAGS} ${CXXINCLUDES} -c -o $*.o $<

container_selectors.o: 


### BUILDING TEST DRIVERS AND DEPENDENCIES OF TEST DRIVER

test: ${TEST_DRIVERS}
	for file in ${TEST_DRIVERS}; do echo "******* TESTING $$file *********"; ./$$file; done

%.t: %.t.o %.o
	${LD} ${LDFLAGS} -o $*.t $^

container_selectors.t.o:

%.t.o: %.t.cpp %.hpp
	${CXX}  ${CXXFLAGS} ${CXXINCLUDES} -c -o $*.t.o $<

container_selectors.t:


## UTILITIES

clean: testclean

testclean:
	- rm *.t.o
	- rm *.t

realclean: clean
	- rm *.o
	- rm ${PACKAGE_LIBRARY}

