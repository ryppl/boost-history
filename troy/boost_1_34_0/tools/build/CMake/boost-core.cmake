##########################################################################
# Boost core support                                                     #
##########################################################################

# Put the libs and bins that get built into directories at the top of
# the build tree rather than in hard to find leaf directories
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# append to a variable
macro(append varname)
  set(${varname} ${${varname}} ${ARGN})
endmacro(append varname)

# Defines a Boost library subproject (e.g., for Boost.Python). Use as:
#
#   boost_library_subproject(libname, subdir1, subdir2, ...)
#
# where libname is the name of the library (e.g., Python, or
# Filesystem) and subdir1, subdir2, etc. are the subdirectories that
# contain CMakeLists files.
#
# This macro will define an option BUILD_BOOST_LIBNAME (which defaults
# to ON). When the option is true, this macro will include the
# subdirectories; otherwise, none of the subdirectories will be
# included, so the library itself will not be built, installed, or
# tested.
macro(boost_library_subproject libname)
  string(TOUPPER "BUILD_BOOST_${libname}" BOOST_LIB_OPTION_NAME)
  option("${BOOST_LIB_OPTION_NAME}" "Build the Boost.${libname} library" ON)
  if ("${BOOST_LIB_OPTION_NAME}")
    foreach(SUBDIR ${ARGN})
      add_subdirectory("${SUBDIR}")
    endforeach(SUBDIR ${ARGN})
  endif ("${BOOST_LIB_OPTION_NAME}")
endmacro(boost_library_subproject)


macro(boost_add_library libname)
  # An internal flag used to keep track of the state of the keyword
  # argument parser
  set(BOOST_ADDLIB_PARSING_VAR "SOURCES")

  # Whether we are parsing options for shared libraries or static
  # libraries (an empty variable needs both)
  set(BOOST_ADDLIB_SHARED_OR_STATIC) 

  # Source files used to build this library
  set(BOOST_ADDLIB_SOURCES)

  # Libraries that this library depends on
  set(BOOST_ADDLIB_LIBRARIES)

  # Compile flags for the static library
  set(BOOST_ADDLIB_STATIC_COMPILE_FLAGS "")

  # Compile flags for the shared library
  set(BOOST_ADDLIB_SHARED_COMPILE_FLAGS "")

  # Whether we should build a static library
  set(BOOST_ADDLIB_STATIC_REQ FALSE)

  # Whether we should build a shared library
  set(BOOST_ADDLIB_SHARED_REQ FALSE)

  # Categorize each of the arguments
  foreach(ARG ${ARGN})
    # Handle keywords
    if (${ARG} STREQUAL "SOURCES")
      set(BOOST_ADDLIB_PARSING_VAR "SOURCES")
    elseif (${ARG} STREQUAL "LIBRARIES")
      set(BOOST_ADDLIB_PARSING_VAR "LIBRARIES")
    elseif (${ARG} STREQUAL "STATIC")
      set(BOOST_ADDLIB_STATIC_REQ TRUE)
      set(BOOST_ADDLIB_SHARED_OR_STATIC "STATIC")
    elseif (${ARG} STREQUAL "SHARED")
      set(BOOST_ADDLIB_SHARED_REQ TRUE)
      set(BOOST_ADDLIB_SHARED_OR_STATIC "SHARED")
    elseif (${ARG} STREQUAL "COMPILE_FLAGS")
      set(BOOST_ADDLIB_PARSING_VAR "COMPILE_FLAGS")

    # Handle arguments based on the last keyword we saw
    elseif(${BOOST_ADDLIB_PARSING_VAR} STREQUAL "SOURCES")
      append(BOOST_ADDLIB_SOURCES ${ARG})
    elseif (${BOOST_ADDLIB_PARSING_VAR} STREQUAL "LIBRARIES")
      append(BOOST_ADDLIB_LIBRARIES ${ARG})
    elseif (${BOOST_ADDLIB_PARSING_VAR} STREQUAL "COMPILE_FLAGS")
      if (BOOST_ADDLIB_SHARED_OR_STATIC STREQUAL "STATIC")
        append(BOOST_ADDLIB_STATIC_COMPILE_FLAGS ${ARG})
      elseif (BOOST_ADDLIB_SHARED_OR_STATIC STREQUAL "SHARED")
	append(BOOST_ADDLIB_SHARED_COMPILE_FLAGS ${ARG})
      else (BOOST_ADDLIB_SHARED_OR_STATIC STREQUAL "STATIC")
	append(BOOST_ADDLIB_STATIC_COMPILE_FLAGS ${ARG})
	append(BOOST_ADDLIB_SHARED_COMPILE_FLAGS ${ARG})
      endif (BOOST_ADDLIB_SHARED_OR_STATIC STREQUAL "STATIC")
    endif (${ARG} STREQUAL "SOURCES")
  endforeach(ARG ${ARGN})

  # If neither STATIC nor SHARED was provided, we build both.
  if (NOT BOOST_ADDLIB_STATIC_REQ)
    if (NOT BOOST_ADDLIB_SHARED_REQ)
      set(BOOST_ADDLIB_STATIC_REQ TRUE)
      set(BOOST_ADDLIB_SHARED_REQ TRUE)
    endif (NOT BOOST_ADDLIB_SHARED_REQ)
  endif (NOT BOOST_ADDLIB_STATIC_REQ)

  # Create the static library target
  if (BOOST_ADDLIB_STATIC_REQ AND BUILD_STATIC_LIBS)
    add_library("${libname}-static" STATIC ${BOOST_ADDLIB_SOURCES})
    set_target_properties("${libname}-static" 
      PROPERTIES OUTPUT_NAME "${libname}"
                 CLEAN_DIRECT_OUTPUT 1
                 COMPILE_FLAGS "${BOOST_ADDLIB_STATIC_COMPILE_FLAGS}")
    if (BOOST_ADDLIB_LIBRARIES)
      target_link_libraries ("${libname}-static" ${BOOST_ADDLIB_LIBRARIES}-static)
    endif (BOOST_ADDLIB_LIBRARIES)
    install(TARGETS "${libname}-static" DESTINATION lib)
  endif (BOOST_ADDLIB_STATIC_REQ AND BUILD_STATIC_LIBS)

  # Create the shared library target
  if (BOOST_ADDLIB_SHARED_REQ AND BUILD_SHARED_LIBS)
    add_library("${libname}-shared" SHARED ${BOOST_ADDLIB_SOURCES})
    set_target_properties("${libname}-shared" 
      PROPERTIES OUTPUT_NAME "${libname}"
                 CLEAN_DIRECT_OUTPUT 1
                 COMPILE_FLAGS "${BOOST_ADDLIB_SHARED_COMPILE_FLAGS}")
    if (BOOST_ADDLIB_LIBRARIES)
      target_link_libraries ("${libname}-shared" ${BOOST_ADDLIB_LIBRARIES}-shared)
    endif (BOOST_ADDLIB_LIBRARIES)
    install(TARGETS "${libname}-shared" DESTINATION lib)
  endif (BOOST_ADDLIB_SHARED_REQ AND BUILD_SHARED_LIBS)
endmacro(boost_add_library)
