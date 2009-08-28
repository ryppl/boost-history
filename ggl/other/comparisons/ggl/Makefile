# Modify these paths for other locations
GGL = ../../../boost
SRC_SHP = ../common/shapelib-1.2.10


INC = $(GGL)

CXX = gcc -O3
CXXFLAGS = -I$(INC) -I$(SRC_SHP) -I../common -I../../../..

GGL_CXXFLAGS = -I$(INC)

DEP = ../common/common.hpp ../common/read_shapefile.hpp
RELEASE = ../release




all: $(RELEASE)/ggl_check

OBJS = shpopen.o dbfopen.o ggl_check.o
	
	
	
$(RELEASE)/ggl_check: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(RELEASE)/ggl_check -lstdc++
	

ggl_check.o: ggl_check.cpp $(DEP)
	$(CXX) -c $(CXXFLAGS) ggl_check.cpp

shpopen.o: $(SRC_SHP)/shpopen.c
	$(CXX) -c $(CXXFLAGS) $(SRC_SHP)/shpopen.c

dbfopen.o: $(SRC_SHP)/dbfopen.c
	$(CXX) -c $(CXXFLAGS) $(SRC_SHP)/dbfopen.c
