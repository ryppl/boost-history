# Modify these paths for other locations

SRC = /c/svn/ggl/other/comparison_star_comb

TL = $(SRC)/terralib/src/terralib/kernel

CXX = gcc -O3 -w
FLAGS = -DTL_DLL= -I$(TL) -I$(SRC)/common

RELEASE = ../release

all: $(RELEASE)/terralib_starcomb

OBJS =  TeAsciiFile.o TeBox.o TeErrorLog.o TeException.o TeFragmentation.o TeGeometry.o \
	TeMultiGeometry.o TeGeometryAlgorithms.o TeIntersector.o TeStdFile.o TeUtils.o \
	TeCentroid.o TeOverlay.o TeOverlayUtils.o \
	terralib_starcomb.o
	
	
	
$(RELEASE)/terralib_starcomb: $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $(RELEASE)/terralib_starcomb -lstdc++
	

terralib_starcomb.o: $(SRC)/terralib/terralib_starcomb.cpp $(SRC)/common/starcomb.hpp
	$(CXX) -c $(FLAGS) $(SRC)/terralib/terralib_starcomb.cpp


# TERRALIB

TeAsciiFile.o: $(TL)/TeAsciiFile.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeAsciiFile.cpp

TeBox.o: $(TL)/TeBox.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeBox.cpp

TeErrorLog.o: $(TL)/TeErrorLog.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeErrorLog.cpp

TeException.o: $(TL)/TeException.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeException.cpp

TeFragmentation.o: $(TL)/TeFragmentation.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeFragmentation.cpp

TeGeometry.o: $(TL)/TeGeometry.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeGeometry.cpp

TeMultiGeometry.o: $(TL)/TeMultiGeometry.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeMultiGeometry.cpp

TeGeometryAlgorithms.o: $(TL)/TeGeometryAlgorithms.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeGeometryAlgorithms.cpp

TeIntersector.o: $(TL)/TeIntersector.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeIntersector.cpp

TeStdFile.o: $(TL)/TeStdFile.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeStdFile.cpp

TeUtils.o: $(TL)/TeUtils.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeUtils.cpp

TeCentroid.o: $(TL)/TeCentroid.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeCentroid.cpp

TeOverlay.o: $(TL)/TeOverlay.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeOverlay.cpp

TeOverlayUtils.o: $(TL)/TeOverlayUtils.cpp
	$(CXX) -c $(FLAGS) $(TL)/TeOverlayUtils.cpp

