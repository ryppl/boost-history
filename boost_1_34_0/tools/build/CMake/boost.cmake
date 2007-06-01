# 
#  organization point for includes 
#
include(list_contains)
include(parse_arguments)
include(CheckCXXSourceCompiles)
if(CMAKE_COMPILER_IS_GNUCC)
  add_definitions(-pipe)
endif(CMAKE_COMPILER_IS_GNUCC)
include(boost-core)
include(boost-testing)

