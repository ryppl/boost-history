# Modify these paths for other locations
WYKOBI = /c/svn/geolib/trunk/src/cpp/library/gl/geometry/comparisons/wykobi/wykobi
SRC_SHP = ../common/shapelib-1.2.10


INC = $(WYKOBI)

CXX = gcc -O3
CXXFLAGS = -I$(INC) -I$(SRC_SHP) -I../common

WYKOBI_CXXFLAGS = -I$(INC)

DEP = ../common/common.hpp ../common/read_shapefile.hpp
RELEASE = ../release




all: $(RELEASE)/wykobi_check

OBJS = shpopen.o dbfopen.o wykobi_check.o
	
	
	
$(RELEASE)/wykobi_check: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(RELEASE)/wykobi_check -lstdc++
	

wykobi_check.o: wykobi_check.cpp $(DEP)
	$(CXX) -c $(CXXFLAGS) wykobi_check.cpp

shpopen.o: $(SRC_SHP)/shpopen.c
	$(CXX) -c $(CXXFLAGS) $(SRC_SHP)/shpopen.c

dbfopen.o: $(SRC_SHP)/dbfopen.c
	$(CXX) -c $(CXXFLAGS) $(SRC_SHP)/dbfopen.c
