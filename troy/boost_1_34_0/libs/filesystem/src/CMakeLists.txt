boost_add_library(
  boost_filesystem
  SOURCES exception.cpp operations.cpp path.cpp portability.cpp utf8_codecvt_facet.cpp
  STATIC
  SHARED COMPILE_FLAGS "-DBOOST_FILESYSTEM_DYN_LINK=1")