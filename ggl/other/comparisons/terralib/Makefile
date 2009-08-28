# Modify these paths for other locations

GGLCOMP = /c/svn/geolib/trunk/src/cpp/library/gl/other/comparisons

TL = $(GGLCOMP)/terralib/src/terralib/kernel
SRC = $(TL)/src
SRC_SHP = $(GGLCOMP)/common/shapelib-1.2.10


INC = $(TL)

CXX = gcc -O3 -w
CXXFLAGS = -DTL_DLL= -I$(INC) -I$(SRC_SHP) -I$(GGLCOMP)/common

TL_CXXFLAGS = -I$(INC) -DTL_DLL=

DEP = $(GGLCOMP)/common/common.hpp $(GGLCOMP)/common/read_shapefile.hpp
RELEASE = ../release


all: $(RELEASE)/terralib_check

OBJS =  TeAsciiFile.o TeBox.o TeErrorLog.o TeException.o TeFragmentation.o TeGeometry.o \
	TeMultiGeometry.o TeGeometryAlgorithms.o TeIntersector.o TeStdFile.o TeUtils.o \
	TeCentroid.o TeOverlay.o TeOverlayUtils.o \
	shpopen.o dbfopen.o \
	terralib_check.o
	
	
	
$(RELEASE)/terralib_check: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(RELEASE)/terralib_check -lstdc++
	

terralib_check.o: $(GGLCOMP)/terralib/terralib_check.cpp $(DEP)
	$(CXX) -c $(CXXFLAGS) $(GGLCOMP)/terralib/terralib_check.cpp

shpopen.o: $(SRC_SHP)/shpopen.c
	$(CXX) -c $(CXXFLAGS) $(SRC_SHP)/shpopen.c

dbfopen.o: $(SRC_SHP)/dbfopen.c
	$(CXX) -c $(CXXFLAGS) $(SRC_SHP)/dbfopen.c

# TERRALIB

TeAsciiFile.o: $(TL)/TeAsciiFile.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeAsciiFile.cpp

TeBox.o: $(TL)/TeBox.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeBox.cpp

TeErrorLog.o: $(TL)/TeErrorLog.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeErrorLog.cpp

TeException.o: $(TL)/TeException.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeException.cpp

TeFragmentation.o: $(TL)/TeFragmentation.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeFragmentation.cpp

TeGeometry.o: $(TL)/TeGeometry.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeGeometry.cpp

TeMultiGeometry.o: $(TL)/TeMultiGeometry.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeMultiGeometry.cpp

TeGeometryAlgorithms.o: $(TL)/TeGeometryAlgorithms.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeGeometryAlgorithms.cpp

TeIntersector.o: $(TL)/TeIntersector.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeIntersector.cpp

TeStdFile.o: $(TL)/TeStdFile.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeStdFile.cpp

TeUtils.o: $(TL)/TeUtils.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeUtils.cpp

TeCentroid.o: $(TL)/TeCentroid.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeCentroid.cpp

TeOverlay.o: $(TL)/TeOverlay.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeOverlay.cpp

TeOverlayUtils.o: $(TL)/TeOverlayUtils.cpp
	$(CXX) -c $(TL_CXXFLAGS) $(TL)/TeOverlayUtils.cpp

