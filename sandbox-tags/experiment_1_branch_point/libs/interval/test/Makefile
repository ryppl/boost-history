CXX=g++
CXXFLAGS=-I../../../ -I../../../../boost -O2

CXXFLAGS+= -DBOOST_INCLUDE_MAIN

ifeq ($(CXX), g++)
  CXXFLAGS+= -D_CPP_NUMERIC_LIMITS -DBOOST_NO_LIMITS
  CXXFLAGS+= -W -Wall
else
# CXXFLAGS+= -pto # to prevent SunPRO from creating its SunWS_cache directories
endif

TARGETS= interval_test add cmp det fmod mul overflow pi pow io

all: $(TARGETS)

test:
	./interval_test
	./add
	./cmp
	./det
	./fmod
	./mul
	./overflow
	./pi
	./pow
	./io

clean:
	rm -f $(TARGETS)

