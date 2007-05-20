#
# This one has the STATIC_TAG to test static/shared compiling differently
#

boost_library_project(
  A
)

boost_library(a 
  lib.cpp
  STATIC_COMPILE_FLAGS "-DA_COMPILE_FLAGS -DA_STATIC_COMPILE_FLAGS -DLINKTYPE=static"
  SHARED_COMPILE_FLAGS "-DA_COMPILE_FLAGS -DA_SHARED_COMPILE_FLAGS -DLINKTYPE=shared"

  STATIC_COMPILE_REQUIREMENTS "-I${CMAKE_CURRENT_SOURCE_DIR} -DA_COMPILE_REQUIREMENTS -DA_STATIC_COMPILE_REQUIREMENTS"
  SHARED_COMPILE_REQUIREMENTS "-I${CMAKE_CURRENT_SOURCE_DIR} -DA_COMPILE_REQUIREMENTS -DA_SHARED_COMPILE_REQUIREMENTS"
  STATIC_TAG  # tds: without STATIC_TAG (append -s to lib name) this fails on darwin.  
              # also true for windows?
)

boost_test_run(test_static DEPENDS a-static)
boost_test_run(test_shared DEPENDS a-shared)
