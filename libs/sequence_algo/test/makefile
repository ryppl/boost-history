CXX = g++
# CXX = g++-3.0

FLAGS = -W -Wall -O3
TIMER_FLAGS = -W -Wall -O3
#TIMER_FLAGS = -W -Wall -DONLY_VECTOR -O3

all: test

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

minmax: minmax.cpp minmax.hpp
	$(CXX) -o $@ $(FLAGS) minmax.cpp

minmax_timer: minmax_timer.cpp minmax.hpp
	$(CXX) -o $@ $(TIMER_FLAGS) minmax_timer.cpp

clean:
	- rm minmax minmax_timer
