CXX=g++
CXXFLAGS=-I../../../ -I../../../../boost -O2

CXXFLAGS+= -DBOOST_INCLUDE_MAIN

ifeq ($(CXX), g++)
  CXXFLAGS+= -Wno-long-double -D_CPP_NUMERIC_LIMITS
  CXXFLAGS+= -W -Wall
else
  CXXFLAGS+= -pto
endif

TARGETS= interval_test add cmp det fmod mul overflow pi pow

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

