CXX = g++-3.0
CXXFLAGS += -W -Wall -pedantic -O -I../..
# CXXFLAGS += -W -Wall -pedantic -g -I../..

all: testing_fixed_external testing_fixed_range \
     testing_fixed_aggregate testing_fixed_allocated \
     testing_variable_incr \
     testing_variable_grow testing_variable_grow_shrink \
     testing_std_vector
	./testing_fixed_aggregate
	./testing_fixed_allocated
	./testing_fixed_external
	./testing_fixed_range
	./testing_variable_incr
	./testing_variable_grow
	./testing_variable_grow_shrink
	./testing_std_vector

BOOST = ../../boost
SIZE  = 100000

testing_fixed_aggregate: testing_vector.cpp $(BOOST)/policy_vector.hpp
	$(CXX) $(CXXFLAGS) -DPOLICY_VECTOR_TESTING_FIXED_AGGREGATE \
   -DPOLICY_VECTOR_DEFAULT_SIZE=$(SIZE) -o $@ testing_vector.cpp

testing_fixed_allocated: testing_vector.cpp $(BOOST)/policy_vector.hpp
	$(CXX) $(CXXFLAGS) -DPOLICY_VECTOR_TESTING_FIXED_ALLOCATED \
   -DPOLICY_VECTOR_DEFAULT_SIZE=$(SIZE) -o $@ testing_vector.cpp

testing_fixed_external: testing_vector.cpp $(BOOST)/policy_vector.hpp
	$(CXX) $(CXXFLAGS) -DPOLICY_VECTOR_TESTING_FIXED_EXTERNAL_STATIC_ARRAY \
   -DPOLICY_VECTOR_DEFAULT_SIZE=$(SIZE) -o $@ testing_vector.cpp

testing_fixed_range: testing_vector.cpp $(BOOST)/policy_vector.hpp
	$(CXX) $(CXXFLAGS) -DPOLICY_VECTOR_TESTING_FIXED_EXTERNAL_RANGE \
   -DPOLICY_VECTOR_DEFAULT_SIZE=$(SIZE) -o $@ testing_vector.cpp

testing_variable_incr: testing_vector.cpp $(BOOST)/policy_vector.hpp
	$(CXX) $(CXXFLAGS) -DPOLICY_VECTOR_TESTING_VARIABLE_INCR \
   -DPOLICY_VECTOR_DEFAULT_SIZE=$(SIZE) -o $@ testing_vector.cpp

testing_variable_grow: testing_vector.cpp $(BOOST)/policy_vector.hpp
	$(CXX) $(CXXFLAGS) -DPOLICY_VECTOR_TESTING_VARIABLE_GROW \
   -DPOLICY_VECTOR_DEFAULT_SIZE=$(SIZE) -o $@ testing_vector.cpp

testing_variable_grow_shrink: testing_vector.cpp $(BOOST)/policy_vector.hpp
	$(CXX) $(CXXFLAGS) -DPOLICY_VECTOR_TESTING_VARIABLE_GROW_SHRINK \
   -DPOLICY_VECTOR_DEFAULT_SIZE=$(SIZE) -o $@ testing_vector.cpp

testing_std_vector: testing_vector.cpp $(BOOST)/policy_vector.hpp
	$(CXX) $(CXXFLAGS) -DPOLICY_VECTOR_TESTING_STD_VECTOR \
   -DPOLICY_VECTOR_DEFAULT_SIZE=$(SIZE) -o $@ testing_vector.cpp

clean:
	- rm -f testing_fixed_aggregate
	- rm -f testing_fixed_allocated
	- rm -f testing_fixed_external
	- rm -f testing_fixed_range
	- rm -f testing_variable_incr
	- rm -f testing_variable_grow
	- rm -f testing_variable_grow_shrink
	- rm -f testing_std_vector
	- rm -f testing_vector

