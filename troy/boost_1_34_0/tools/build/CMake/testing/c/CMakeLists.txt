boost_library_project(
  C
)

boost_library(c
  lib.cpp
  COMPILE_FLAGS -DC_COMPILE_FLAGS
  STICKY_COMPILE_FLAGS -DC_STICKY_COMPILE_FLAGS
  STATIC_COMPILE_FLAGS -DLINKTYPE=static
  STICKY_STATIC_COMPILE_FLAGS -DC_STICKY_STATIC_COMPILE_FLAGS
  SHARED_COMPILE_FLAGS -DLINKTYPE=shared
  STICKY_SHARED_COMPILE_FLAGS -DC_STICKY_SHARED_COMPILE_FLAGS
  DEPENDS a
)

boost_test_run(test_shared DEPENDS c-shared a-shared)
boost_test_run(test_static DEPENDS c-static a-static)

boost_test_run(astatic_cshared DEPENDS c-shared a-static)
boost_test_run(ashared_cstatic DEPENDS c-static a-shared)
