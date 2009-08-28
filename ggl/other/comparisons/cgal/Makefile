# Modify these paths for other locations
CGAL = ../../../../../CGAL-3.4
SRC = $(CGAL)/src
SRC_SHP = ../common/shapelib-1.2.10

LIB_LOCAL = /usr/local/lib
LIB_BOOST = /home/barend/boost_1_39_0/stage/lib
BOOST_LIB = libboost_thread-gcc43-mt-1_39

INC = $(CGAL)/include

CXX = gcc -O3 -w
CXXFLAGS = -I$(INC) -I$(SRC_SHP) -I../common  -I../../../../../boost_1_39_0/

CGAL_CXXFLAGS = -I$(INC)

DEP = ../common/common.hpp ../common/read_shapefile.hpp
RELEASE = ../release




all: $(RELEASE)/cgal_check

OBJS = shpopen.o dbfopen.o cgal_check.o

# MP_Float.o assertions.o
	
	
# for LINUX on home-pc Barend:	
$(RELEASE)/cgal_check: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(RELEASE)/cgal_check -rdynamic -L$(LIB_LOCAL) -L$(LIB_BOOST) \
	$(LIB_LOCAL)/libCGAL_Core.so $(LIB_LOCAL)/libCGAL.so $(LIB_LOCAL)/libmpfr.so $(LIB_LOCAL)/libgmp.so \
	$(LIB_BOOST)/$(BOOST_LIB).so \
	$(LIB_LOCAL)/libCGAL_Core.so $(LIB_LOCAL)/libCGAL.so $(LIB_LOCAL)/libmpfr.so $(LIB_LOCAL)/libgmp.so \
	$(LIB_BOOST)/$(BOOST_LIB).so \
	-Wl,-rpath,$(LIB_LOCAL):$(LIB_BOOST) 
	

cgal_check.o: cgal_check.cpp $(DEP)
	$(CXX) -c $(CXXFLAGS) cgal_check.cpp

shpopen.o: $(SRC_SHP)/shpopen.c
	$(CXX) -c $(CXXFLAGS) $(SRC_SHP)/shpopen.c

dbfopen.o: $(SRC_SHP)/dbfopen.c
	$(CXX) -c $(CXXFLAGS) $(SRC_SHP)/dbfopen.c


# For Windows/MinGW the two files below are enough (to compile WITHOUT intersection-test)
MP_Float.o: $(SRC)/CGAL/MP_Float.cpp
	$(CXX) -c $(CGAL_CXXFLAGS) $(SRC)/CGAL/MP_Float.cpp

assertions.o: $(SRC)/CGAL/assertions.cpp
	$(CXX) -c $(CGAL_CXXFLAGS) $(SRC)/CGAL/assertions.cpp

