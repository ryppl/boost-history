
cmake_minimum_required(VERSION 2.6)

# Grab BOOST_ROOT from the env and use it to find the Boost CMake stuff.
set(BOOST_ROOT $ENV{BOOST_ROOT})
if(NOT BOOST_ROOT)
    message(FATAL_ERROR "Set $BOOST_ROOT before building")
endif()
set(Boost_INCLUDE_DIRS ${BOOST_ROOT})

# Include the Boost CMake core.
list(APPEND CMAKE_MODULE_PATH ${BOOST_ROOT}/tools/build/CMake)
include(BoostUtils)
include(BoostConfig)
include(BoostCore)
include(BoostDocs)
include(BoostTesting)

# Define the library project
# TODO: This doesn't actually seem to do very much if we're out of the build.
boost_library_project(
  Graphs
  # SRCDIRS test
  TESTDIRS test
  DESCRIPTION "Generic graph data structures and algorithms"
  AUTHORS "Andrew Sutton < asutton -at- cs.kent.edu"
  MODULAR
)
set(Graphs_ROOT ${CMAKE_SOURCE_DIR})
set(Graphs_INCLUDE_DIRS ${Graphs_ROOT}/include)

add_subdirectory(test)
