CXX = g++-3.0
CXXFLAGS += -W -Wall -O3 -I../../../
# CXXFLAGS += -W -Wall -pedantic -O3 -I../../../
# CXXFLAGS += -W -Wall -pedantic -O3 -I../../../
# CXXFLAGS += -W -Wall -pedantic -g -I../../../
# CXXFLAGS += -W -Wall -g -I../../../

all:  testing_counting_sort testing_radix_sort_all testing_block_sort
	# - ./testing_counting_sort
	- ./testing_radix_sort_unsigned_builtin
	- ./testing_radix_sort_signed_builtin
	- ./testing_radix_sort_float_builtin
	- ./testing_radix_sort_char_array
	- ./testing_radix_sort_unsigned_char_array
	- ./testing_radix_sort_unsigned_int_array 
	# - ./testing_block_sort

testing_radix_sort_all: testing_radix_sort_unsigned_builtin \
   testing_radix_sort_signed_builtin \
   testing_radix_sort_char_array \
   testing_radix_sort_unsigned_char_array \
   testing_radix_sort_unsigned_int_array

BOOST = ../../../boost
SIZE  = 1000000

testing_counting_sort: testing_counting_sort.cpp
	$(CXX) $(CXXFLAGS) -DSIZE=$(SIZE) -o $@ $<

testing_radix_sort_unsigned_builtin: testing_radix_sort.cpp
	$(CXX) $(CXXFLAGS) -DSIZE=$(SIZE) -DTIME_UNSIGNED_BUILTIN -o $@ $<

testing_radix_sort_signed_builtin: testing_radix_sort.cpp
	$(CXX) $(CXXFLAGS) -DSIZE=$(SIZE) -DTIME_SIGNED_BUILTIN -o $@ $<

testing_radix_sort_float_builtin: testing_radix_sort.cpp
	$(CXX) $(CXXFLAGS) -DSIZE=$(SIZE) -DTIME_FLOAT_BUILTIN -o $@ $<

testing_radix_sort_bool_array : testing_radix_sort.cpp
	$(CXX) $(CXXFLAGS) -DSIZE=$(SIZE) -DTIME_BOOL_ARRAY -o $@ $<

testing_radix_sort_char_array: testing_radix_sort.cpp
	$(CXX) $(CXXFLAGS) -DSIZE=$(SIZE) -DTIME_CHAR_ARRAY -o $@ $<

testing_radix_sort_unsigned_char_array: testing_radix_sort.cpp
	$(CXX) $(CXXFLAGS) -DSIZE=$(SIZE) -DTIME_UNSIGNED_CHAR_ARRAY -o $@ $<

SIZE  = 250000
testing_radix_sort_unsigned_int_array: testing_radix_sort.cpp
	$(CXX) $(CXXFLAGS) -DSIZE=$(SIZE) -DTIME_UNSIGNED_INT_ARRAY -o $@ $<

SIZE  = 1000000
testing_block_sort: testing_block_sort.cpp $(BOOST)/linear_sort.hpp
	$(CXX) $(CXXFLAGS) -DSIZE=$(SIZE) -o $@ testing_block_sort.cpp

clean:
	- rm -f testing_counting_sort
	- rm -f testing_radix_sort_*
	- rm -f testing_block_sort

