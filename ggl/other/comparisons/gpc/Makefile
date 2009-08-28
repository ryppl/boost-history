# Modify these paths for other locations

GGLCOMP = ..
GPC = $(GGLCOMP)/gpc/gpc


SRC = $(GPC)/src
SRC_SHP = $(GGLCOMP)/common/shapelib-1.2.10


CXX = gcc -O3 -w
CXXFLAGS = -I$(GPC) -I$(SRC_SHP) -I$(GGLCOMP)/common  -I../../../..

GPC_CXXFLAGS = -I$(GPC)

DEP = $(GGLCOMP)/common/common.hpp $(GGLCOMP)/common/read_shapefile.hpp
RELEASE = ../release




all: $(RELEASE)/gpc_check 

OBJS = shpopen.o dbfopen.o gpc_check.o gpc.o
	
	
	
$(RELEASE)/gpc_check: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(RELEASE)/gpc_check -lstdc++
	
gpc_check.o: $(GGLCOMP)/gpc/gpc_check.cpp $(DEP)
	$(CXX) -c $(CXXFLAGS) $(GGLCOMP)/gpc/gpc_check.cpp
	
gpc.o: $(GGLCOMP)/gpc/gpc/gpc.c $(DEP)
	$(CXX) -c $(CXXFLAGS) $(GGLCOMP)/gpc/gpc/gpc.c
	

shpopen.o: $(SRC_SHP)/shpopen.c
	$(CXX) -c $(CXXFLAGS) $(SRC_SHP)/shpopen.c

dbfopen.o: $(SRC_SHP)/dbfopen.c
	$(CXX) -c $(CXXFLAGS) $(SRC_SHP)/dbfopen.c

