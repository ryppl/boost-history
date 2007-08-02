CXX=g++
CXXFLAGS=-pedantic -W -Wall -I.

LD=g++

TARGET=dbg_exc_mt

PACKAGE_OBJECTS=container_selectors.o facet_selectors.o \
	facet_functions.o vertex_selectors.o vertex_functions.o \
	halfedge_selectors.o halfedge_functions.o

PACKAGE_LIBRARY=libhalfedge_ds.a

TEST_DRIVERS=container_selectors.t facet_selectors.t \
	facet_functions.t vertex_selectors.t vertex_functions.t \
	halfedge_selectors.t halfedge_functions.t

.SUFFIXES:
.SUFFIXES: ${TARGET}.o .o .cpp .hpp

all: package_library test


### BUILDING PACKAGE LIBRARY AND DEPENDENCIES

package_library: ${PACKAGE_OBJECTS}
	ar -rv ${PACKAGE_LIBRARY} ${PACKAGE_OBJECTS}

%.o: %.cpp %.h
	${CXX} ${CXXFLAGS} ${CXXINCLUDES} -c -o $*.o $<

container_selectors.o: 

facet_selectors.o:  container_selectors.hpp

facet_functions.o:  facet_selectors.hpp

vertex_selectors.o: container_selectors.hpp

vertex_functions.o: vertex_selectors.hpp

halfedge_selectors.o: container_selectors.hpp

halfedge_functions.o: halfedge_functions.hpp \
                      halfedge_selectors.hpp container_selectors.hpp

### BUILDING TEST DRIVERS AND DEPENDENCIES OF TEST DRIVER

test: ${TEST_DRIVERS}
	for file in ${TEST_DRIVERS}; do echo "******* TESTING $$file *********"; ./$$file; done

%.t: %.t.o %.o
	${LD} ${LDFLAGS} -o $*.t $^

container_selectors.t.o: 

facet_selectors.t.o: container_selectors.hpp

facet_functions.t.o:  facet_selectors.hpp

vertex_selectors.t.o: container_selectors.hpp

vertex_functions.t.o: vertex_selectors.hpp

halfedge_selectors.t.o: container_selectors.hpp

halfedge_functions.t.o: halfedge_functions.hpp \
                      halfedge_selectors.hpp container_selectors.hpp

%.t.o: %.t.cpp %.hpp
	${CXX}  ${CXXFLAGS} ${CXXINCLUDES} -c -o $*.t.o $<

container_selectors.t: 

facet_selectors.t: container_selectors.o

facet_functions.t:  facet_selectors.o

vertex_selectors.t: container_selectors.o 

vertex_functions.t: vertex_selectors.o

halfedge_selectors.t: container_selectors.o 

halfedge_functions.t.o: halfedge_selectors.o container_selectors.o

## UTILITIES

clean: realclean testclean

testclean:
	- rm *.t.o
	- rm *.t

realclean: 
	- rm *.o
	- rm ${PACKAGE_LIBRARY}

