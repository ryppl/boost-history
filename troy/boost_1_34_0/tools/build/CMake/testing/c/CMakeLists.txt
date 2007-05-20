boost_library_project(
  C
)

boost_library(c
  lib.cpp
  STATIC_COMPILE_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR} -DC_COMPILE_FLAGS -DLINKTYPE=static"
  SHARED_COMPILE_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR} -DC_COMPILE_FLAGS -DLINKTYPE=shared"

  STATIC_COMPILE_REQUIREMENTS "-I${CMAKE_CURRENT_SOURCE_DIR} -DC_COMPILE_REQUIREMENTS -DC_STATIC_COMPILE_REQUIREMENTS"
  SHARED_COMPILE_REQUIREMENTS "-I${CMAKE_CURRENT_SOURCE_DIR} -DC_COMPILE_REQUIREMENTS -DC_SHARED_COMPILE_REQUIREMENTS"
  DEPENDS a
  LIBRARIES rt
)
 
boost_test_run(test_shared DEPENDS c-shared a-shared)
boost_test_run(test_static DEPENDS c-static a-static)
 
boost_test_run(astatic_cshared DEPENDS a-static c-shared)
boost_test_run(ashared_cstatic DEPENDS a-shared c-static)
