# Modify these paths for other locations

SRC = /c/svn/ggl/other/comparison_star_comb


BP = /c/svn/gtl

CXX = gcc -O3 
FLAGS = -I$(BP)  -I$(SRC)/common

RELEASE = ../release

$(RELEASE)/gtl_starcomb: $(SRC)/gtl/gtl_starcomb.cpp $(SRC)/common/starcomb.hpp
	$(CXX) $(FLAGS) $(SRC)/gtl/gtl_starcomb.cpp -o $(RELEASE)/gtl_starcomb -lstdc++
	
