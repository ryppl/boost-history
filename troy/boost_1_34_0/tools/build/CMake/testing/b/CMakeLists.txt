boost_library_subproject(
  B
)

boost_library(b
  lib.cpp
  COMPILE_FLAGS -DB_COMPILE_FLAGS
  STICKY_COMPILE_FLAGS -DB_STICKY_COMPILE_FLAGS
  STATIC_COMPILE_FLAGS -DLINKTYPE=static
  STICKY_STATIC_COMPILE_FLAGS -DB_STICKY_STATIC_COMPILE_FLAGS
  SHARED_COMPILE_FLAGS -DLINKTYPE=shared
  STICKY_SHARED_COMPILE_FLAGS -DB_STICKY_SHARED_COMPILE_FLAGS
)

boost_test_run(test_shared DEPENDS b-shared a-shared)
boost_test_run(test_static DEPENDS b-static a-static)

boost_test_run(astatic_bshared DEPENDS a-static b-shared)
boost_test_run(ashared_bstatic DEPENDS a-shared b-static)
