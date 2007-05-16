# Find each subdirectory containing a CMakeLists.txt file, and include
# it. This avoids the need to manually list which libraries in Boost
# have CMakeLists.txt files.
file(GLOB BOOST_LIBRARY_CMAKE_FILES 
     RELATIVE "${Boost_SOURCE_DIR}/libs" "*/CMakeLists.txt")
MESSAGE(STATUS "Scanning subdirectories:")
foreach(BOOST_LIB_CMAKE_FILE ${BOOST_LIBRARY_CMAKE_FILES})
  string(REGEX MATCH "^[^/]*" BOOST_LIB_DIR "${BOOST_LIB_CMAKE_FILE}")
  add_subdirectory("${BOOST_LIB_DIR}")
  MESSAGE("   + ${BOOST_LIB_DIR}")
endforeach(BOOST_LIB_CMAKE_FILE ${BOOST_LIBRARY_CMAKE_FILES})
