##########################################################################
# Boost Configuration Information                                        #
##########################################################################
#
# This module defines several variables that provide information about
# the target compiler and platform.
#
# Variables defined:
# 
#   BOOST_TOOLSET:
#     The Boost toolset name, used by the library version mechanism to
#     encode the compiler and version into the name of the
#     library. This toolset name will correspond with Boost.Build
#     version 2's toolset name.
#
#   MULTI_THREADED_COMPILE_FLAGS:
#     Compilation flags when building multi-threaded programs.
#
#   MULTI_THREADED_LINK_FLAGS:
#     Linker flags when building multi-threaded programs.
#

# Toolset detection.
if (NOT BOOST_TOOLSET)
  if (MSVC60)
    set(BOOST_TOOLSET "vc6")
  elseif(MSVC70)
    set(BOOST_TOOLSET "vc7")
  elseif(MSVC71)
    set(BOOST_TOOLSET "vc71")
  elseif(MSVC80)
    set(BOOST_TOOLSET "vc80")
  elseif(MSVC)
  elseif(BORLAND)
    set(BOOST_TOOLSET "bcb")
  elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Execute GCC with the -dumpversion option, to give us a version string
    execute_process(
      COMMAND ${CMAKE_CXX_COMPILER} "-dumpversion" 
      OUTPUT_VARIABLE GCC_VERSION_STRING)
    
    # Match only the major and minor versions of the version string
    string(REGEX MATCH "[0-9]+.[0-9]+" GCC_MAJOR_MINOR_VERSION_STRING
      "${GCC_VERSION_STRING}")
    
    # Strip out the period between the major and minor versions
    string(REGEX REPLACE "\\." "" BOOST_VERSIONING_GCC_VERSION
      "${GCC_MAJOR_MINOR_VERSION_STRING}")
    
    # Set the GCC versioning toolset
    set(BOOST_TOOLSET "gcc${BOOST_VERSIONING_GCC_VERSION}")
  endif(MSVC60)
endif (NOT BOOST_TOOLSET)

# Determine whether we should use library versioning by default
if(BOOST_TOOLSET)
  set(BUILD_VERSIONED_BY_DEFAULT ON)
else(BOOST_TOOLSET)
  set(BUILD_VERSIONED_BY_DEFAULT OFF)
endif(BOOST_TOOLSET)

option(
  BUILD_VERSIONED 
  "Use versioned library names, e.g., boost_filesystem-gcc41-1_34" 
  ${BUILD_VERSIONED_BY_DEFAULT})

if(BUILD_VERSIONED)
  # Set BOOST_TOOLSET to the string that describes the
  # Boost.Build toolset. This is used as part of the versioned library
  # name.
  if(NOT BOOST_TOOLSET)
    message("Unable to determine compiler toolset. Library versioning cannot be used")
    set(BUILD_VERSIONED OFF CACHE BOOL 
      "Use versioned library names, e.g., boost_filesystem-gcc41-1_34" FORCE)
    set(BOOST_LIBRARY_VERSION_STRING "")
    set(BOOST_LIBRARY_VERSION_STRING_DEBUG "")
  endif(NOT BOOST_TOOLSET)
endif(BUILD_VERSIONED)

# Multi-threading support
if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
  set(MULTI_THREADED_COMPILE_FLAGS "-pthreads")
  set(MULTI_THREADED_LINK_LIBS rt)
elseif(CMAKE_SYSTEM_NAME STREQUAL "BeOS")
  # No threading options necessary for BeOS
elseif(CMAKE_SYSTEM_NAME MATCHES ".*BSD")
  set(MULTI_THREADED_COMPILE_FLAGS "-pthread")
elseif(CMAKE_SYSTEM_NAME STREQUAL "DragonFly")
  # DragonFly is  FreeBSD bariant
  set(MULTI_THREADED_COMPILE_FLAGS "-pthread")
elseif(CMAKE_SYSTEM_NAME STREQUAL "IRIX")
  # TODO: GCC on Irix doesn't support multi-threading?
elseif(CMAKE_SYSTEM_NAME STREQUAL "HP-UX")
  # TODO: gcc on HP-UX does not support multi-threading?
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  # No threading options necessary for Mac OS X
elseif(UNIX)
  # Assume -pthread and -lrt on all other variants
  set(MULTI_THREADED_COMPILE_FLAGS "-pthread -D_REENTRANT")
  set(MULTI_THREADED_LINK_FLAGS "")  
  set(MULTI_THREADED_LINK_LIBS pthread rt)
endif(CMAKE_SYSTEM_NAME STREQUAL "SunOS")

