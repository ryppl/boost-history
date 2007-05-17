# 
#  organization point for includes 
#
include(list_contains)
include(parse_arguments)

include(boost-core)
include(boost-testing)

set(DEBUG FALSE)
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
    
SET(RUN_SELFTESTS FALSE)

if(RUN_SELFTESTS)
  trace("Start selftests")
  file (GLOB testfiles_ "${CMAKE_SOURCE_DIR}/tools/build/CMake/tests/*.cmake")
  foreach(file_ ${testfiles_})
    trace("### ${file_}")
    include(${file_})
  endforeach(file_ ${testfiles_})
  trace("End selftests")
endif(RUN_SELFTESTS)

