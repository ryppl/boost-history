boost_library_project(
  B
)

boost_library(b
  lib.cpp
  STATIC_COMPILE_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR} -DB_COMPILE_FLAGS -DLINKTYPE=static"
  SHARED_COMPILE_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR} -DB_COMPILE_FLAGS -DLINKTYPE=shared"

  STATIC_COMPILE_REQUIREMENTS "-I${CMAKE_CURRENT_SOURCE_DIR} -DB_COMPILE_REQUIREMENTS -DB_STATIC_COMPILE_REQUIREMENTS"
  SHARED_COMPILE_REQUIREMENTS "-I${CMAKE_CURRENT_SOURCE_DIR} -DB_COMPILE_REQUIREMENTS -DB_SHARED_COMPILE_REQUIREMENTS"
  DEPENDS a
)
 
boost_test_run(test_shared DEPENDS b-shared a-shared)
boost_test_run(test_static DEPENDS b-static a-static)
 
boost_test_run(astatic_bshared DEPENDS a-static b-shared)
boost_test_run(ashared_bstatic DEPENDS a-shared b-static)
