# Modify these paths for other locations

SRC = /c/svn/ggl/other/comparison_star_comb

#GGL = /c/svn/geolib/trunk/src/cpp/library/gl
GGL = /c/svn/ggl/formal_review

CXX = gcc -O3
FLAGS = -I$(GGL)/boost -I$(SRC)/common -I$(GGL)

RELEASE = ../release
	
$(RELEASE)/ggl_starcomb: $(SRC)/ggl/ggl_starcomb.cpp $(SRC)/common/starcomb.hpp
	$(CXX) $(FLAGS) $(SRC)/ggl/ggl_starcomb.cpp -o $(RELEASE)/ggl_starcomb -lstdc++
	
