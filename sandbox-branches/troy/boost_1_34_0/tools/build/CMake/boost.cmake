# 
#  organization point for includes 
#
include(list_contains)
include(parse_arguments)

include(boost-core)
include(boost-testing)

OPTION(DEBUG "Print lots of gnasty trace output from makefile generation stage" OFF)
macro(TRACE)
  if (DEBUG)
    MESSAGE("~~ " ${ARGN})
  endif (DEBUG)
endmacro(TRACE)

macro(ASSERT value)
  if (NOT ${value})
    MESSAGE("Assertion failed: ${value} = ${${value}}")
    MESSAGE(FATAL_ERROR ${ARGN})
  endif (NOT ${value})
endmacro(ASSERT value)
    

