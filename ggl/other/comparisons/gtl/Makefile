# Modify these paths for other locations
GTL = ../../../../gtl
SRC_SHP = ../common/shapelib-1.2.10


INC = $(GTL)

CXX = gcc -O3
CXXFLAGS = -I$(INC) -I$(SRC_SHP) -I../common -I../../../..

GTL_CXXFLAGS = -I$(INC)

DEP = ../common/common.hpp ../common/read_shapefile.hpp
RELEASE = ../release




all: $(RELEASE)/gtl_check

OBJS = shpopen.o dbfopen.o gtl_check.o
	
	
	
$(RELEASE)/gtl_check: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(RELEASE)/gtl_check -lstdc++
	

gtl_check.o: gtl_check.cpp $(DEP)
	$(CXX) -c $(CXXFLAGS) gtl_check.cpp

shpopen.o: $(SRC_SHP)/shpopen.c
	$(CXX) -c $(CXXFLAGS) $(SRC_SHP)/shpopen.c

dbfopen.o: $(SRC_SHP)/dbfopen.c
	$(CXX) -c $(CXXFLAGS) $(SRC_SHP)/dbfopen.c
