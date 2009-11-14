# Modify these paths for other locations

SRC = /c/svn/ggl/other/comparison_star_comb

CGAL = ../../CGAL-3.4

CXX = gcc -O3 
FLAGS = -I$(CGAL)/include -I$(SRC)/common

OBJS = MP_Float.o assertions.o cgal_starcomb.o
	
RELEASE = ../release


all: $(RELEASE)/cgal_starcomb 

$(RELEASE)/cgal_starcomb: $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $(RELEASE)/cgal_starcomb -lstdc++
	
cgal_starcomb.o: $(SRC)/cgal/cgal_starcomb.cpp $(SRC)/common/starcomb.hpp
	$(CXX) -c $(FLAGS) $(SRC)/cgal/cgal_starcomb.cpp


MP_Float.o: $(CGAL)/src/CGAL/MP_Float.cpp
	$(CXX) -c $(FLAGS) $(CGAL)/src/CGAL/MP_Float.cpp
	
assertions.o: $(CGAL)/src/CGAL/assertions.cpp
	$(CXX) -c $(FLAGS) $(CGAL)/src/CGAL/assertions.cpp

