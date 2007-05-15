add_definitions(-DBOOST_GRAPH_NO_LIB=1)

if (MSVC)
  # Without these flags, MSVC 7.1 and 8.0 crash
  add_definitions(-GR-)
endif (MSVC)

set(BOOST_GRAPH_OPTIONAL_SOURCES "")
set(BOOST_GRAPH_OPTIONAL_LIBRARIES "")

# Try to find the Expat library 
include(FindEXPAT)
if (EXPAT_FOUND)
  # We have Expat, so build the GraphML parser
#  set(BOOST_GRAPH_OPTIONAL_SOURCES 
#      ${BOOST_GRAPH_OPTIONAL_SOURCES} "graphml.cpp")
  include_directories(${EXPAT_INCLUDE_DIRS})
  set(BOOST_GRAPH_OPTIONAL_LIBRARIES 
      "${BOOST_GRAPH_OPTIONAL_LIBRARIES} ${EXPAT_LIBRARIES}")
endif (EXPAT_FOUND)

boost_add_library(
  boost_graph
  SOURCES read_graphviz_spirit.cpp ${BOOST_GRAPH_OPTIONAL_SOURCES}
  LIBRARIES "${BOOST_GRAPH_OPTIONAL_LIBRARIES}"
  STATIC
  SHARED COMPILE_FLAGS "-DBOOST_GRAPH_DYN_LINK=1")