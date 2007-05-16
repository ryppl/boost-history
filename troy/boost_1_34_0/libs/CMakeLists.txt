# Find each subdirectory containing a CMakeLists.txt file, and include
# it. This avoids the need to manually list which libraries in Boost
# have CMakeLists.txt files.

# return a list of directories that we should add_subdirectory()
macro(BOOST_COLLECT_SUBPROJECT_DIRECTORY_NAMES varname)
  file(GLOB BOOST_LIBRARY_CMAKE_FILES 
    RELATIVE "${Boost_SOURCE_DIR}/libs" "*/CMakeLists.txt")
  MESSAGE(STATUS "Scanning subdirectories:")
  foreach(BOOST_LIB_CMAKE_FILE ${BOOST_LIBRARY_CMAKE_FILES})
    get_filename_component(BOOST_LIB_DIR ${BOOST_LIB_CMAKE_FILE} PATH)
    append(${varname} ${BOOST_LIB_DIR})
  endforeach(BOOST_LIB_CMAKE_FILE ${BOOST_LIBRARY_CMAKE_FILES})
endmacro(BOOST_COLLECT_SUBPROJECT_DIRECTORY_NAMES varname)

macro(ADD_SUBDIRECTORIES prefix)
  foreach(subdir ${ARGN})
    MESSAGE("${prefix}${subdir}")
    add_subdirectory(${subdir})
  endforeach(subdir ${ARGN})
endmacro(ADD_SUBDIRECTORIES prefix)

boost_collect_subproject_directory_names(BOOST_SUBPROJECT_DIRS)
set(BOOST_CMAKE_READ_PASS "SRC")
add_subdirectories("   src:  " ${BOOST_SUBPROJECT_DIRS})
set(BOOST_CMAKE_READ_PASS "TEST")
add_subdirectories("   test: " ${BOOST_SUBPROJECT_DIRS})

