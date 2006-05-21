BOOST           = /usr/include/boost_1_30_0
CXX             = g++
EXTRAFLAGS      = -pedantic -Wno-long-long -Wall 
INCLUDES        = -I$(BOOST) -I./../../..
CXXFLAGS        = $(INCLUDES) $(EXTRAFLAGS)

SOURCES = \
  basic.cpp\
  bimap.cpp\
  complex_structs.cpp\
  composite_compares\
  memfun_key.cpp\
  non_default_ctor.cpp\
  sequenced.cpp\

OBJECTS         = ${SOURCES:.cpp=.o}

TARGETS         = ${SOURCES:.cpp=.exe}     

all: $(TARGETS)

%.exe: %.o
	$(CXX) -o $@ $?

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:	
	/bin/rm -rf $(TARGETS) $(OBJECTS)
