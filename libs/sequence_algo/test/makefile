CXX = g++
# CXX = g++3

CXXFLAGS = -W -Wall -O3 -I../../../../boost -I../../../ -DBOOST_INCLUDE_MAIN

test: minmax
	- ./minmax 0
	- ./minmax 1
	- ./minmax 2
	- ./minmax 3
	- ./minmax 100
	- ./minmax 101

SIZE = 100000
REPEATS = 100

timer: minmax_timer
	./minmax_timer $(SIZE) $(REPEATS)

minmax: minmax.cpp ../../../boost/sequence_algo/minmax.hpp
minmax_timer: minmax_timer.cpp ../../../boost/sequence_algo/minmax.hpp

clean:
	- rm minmax minmax_timer
