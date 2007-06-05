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

# Defines a Boost library project (e.g., for Boost.Python). Use as:
#
#   boost_library_project(libname, subdir1, subdir2, ...)
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
macro(boost_library_project libname_)
  parse_arguments(THIS_PROJECT
    "SRCDIRS;TESTDIRS"
    ""
    ${ARGN}
    )

  string(TOUPPER "BUILD_BOOST_${libname_}" BOOST_BUILD_LIB_OPTION)
  if (THIS_PROJECT_SRCDIRS)
    # This Boost library has source directories, so provide an option
    # BUILD_BOOST_LIBNAME that allows one to turn on/off building of
    # the library.
    option(${BOOST_BUILD_LIB_OPTION} 
      "Build Boost.${libname_} (prefer make targets, not this, to build individual libs)" 
      ON)
  else (THIS_PROJECT_SRCDIRS)
    # This Boost library has no source directories, and therefore does
    # not require building. Always enable it (but don't make it an
    # option in the cache).
    set(${BOOST_BUILD_LIB_OPTION} ON)
  endif (THIS_PROJECT_SRCDIRS)

  if(${BOOST_BUILD_LIB_OPTION})
    string(TOLOWER "${libname_}" libname)
    project(${libname})

    if(NOT EXISTS ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})
      file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})
    endif(NOT EXISTS ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})

    # Include each of the source directories
    foreach(SUBDIR ${THIS_PROJECT_SRCDIRS})
      add_subdirectory(${SUBDIR})
    endforeach(SUBDIR ${THIS_PROJECT_SRCDIRS})

    if(BUILD_TESTING AND THIS_PROJECT_TESTDIRS)
      # Testing is enabled globally and this project has some
      # tests. So, add a testing option.
      string(TOUPPER "TEST_BOOST_${libname_}" BOOST_TEST_LIB_OPTION)
      option(${BOOST_TEST_LIB_OPTION} 
        "Enable testing of Boost.${libname_}" 
        ON)

      # Only include the test directories when testing is enabled for
      # this project.
      if(${BOOST_TEST_LIB_OPTION})
        foreach(SUBDIR ${THIS_PROJECT_TESTDIRS})
          add_subdirectory(${SUBDIR})
        endforeach(SUBDIR ${THIS_PROJECT_TESTDIRS})
      endif(${BOOST_TEST_LIB_OPTION})
    endif(BUILD_TESTING AND THIS_PROJECT_TESTDIRS)
  endif(${BOOST_BUILD_LIB_OPTION})
endmacro(boost_library_project)

macro(boost_library_variant_target_name)
  set(VARIANT_TARGET_NAME "")
  set(VARIANT_VERSIONED_NAME "")

  if (BUILD_VERSIONED)
    # If we're using versioning for the names of our generated
    # libraries, detect the full toolset name.
    set(VARIANT_VERSIONED_NAME "-${BOOST_TOOLSET}")
  endif (BUILD_VERSIONED)

  # Add -mt for multi-threaded libraries
  list_contains(variant_is_mt MULTI_THREADED ${ARGN})
  if (variant_is_mt)
    set(VARIANT_TARGET_NAME "${VARIANT_TARGET_NAME}-mt")

    if (BUILD_VERSIONED)
      # If we're creating versioned names, tack on "-mt"
      set(VARIANT_VERSIONED_NAME "${VARIANT_VERSIONED_NAME}-mt")
    endif (BUILD_VERSIONED)
  endif (variant_is_mt)

  # Add -static for static libraries, -shared for shared libraries
  list_contains(variant_is_static STATIC ${ARGN})
  if (variant_is_static)
    set(VARIANT_TARGET_NAME "${VARIANT_TARGET_NAME}-static")
  else (variant_is_static)
    set(VARIANT_TARGET_NAME "${VARIANT_TARGET_NAME}-shared")
  endif (variant_is_static)

  set(VARIANT_ABI_TAG "")

  # TODO: Linking statically to the runtime library
  # TODO: Using debug versions of the standard/runtime support libs
  # TODO: using debug build of Python
  # TODO: STLport rather than default library
  # TODO: STLport's deprecated iostreams

  # Add -debug for debug libraries
  list_contains(variant_is_debug DEBUG ${ARGN})
  if (variant_is_debug)
    set(VARIANT_TARGET_NAME "${VARIANT_TARGET_NAME}-debug")

    if (BUILD_VERSIONED)
      set(VARIANT_ABI_TAG "${VARIANT_ABI_TAG}d")
    endif (BUILD_VERSIONED)
  endif (variant_is_debug)

  if (BUILD_VERSIONED)
    # If there is an ABI tag, append it to the versioned name
    if (VARIANT_ABI_TAG)
      set(VARIANT_VERSIONED_NAME "${VARIANT_VERSIONED_NAME}-${VARIANT_ABI_TAG}")
    endif (VARIANT_ABI_TAG)

    # Append the Boost version number to the versioned name
    if(BOOST_VERSION_SUBMINOR GREATER 0)
      set(VARIANT_VERSIONED_NAME
        "${VARIANT_VERSIONED_NAME}-${BOOST_VERSION_MAJOR}_${BOOST_VERSION_MINOR}_${BOOST_VERSION_SUBMINOR}")
    else(BOOST_VERSION_SUBMINOR GREATER 0)
      set(VARIANT_VERSIONED_NAME 
        "${VARIANT_VERSIONED_NAME}-${BOOST_VERSION_MAJOR}_${BOOST_VERSION_MINOR}")
    endif(BOOST_VERSION_SUBMINOR GREATER 0)
  endif (BUILD_VERSIONED)
endmacro(boost_library_variant_target_name)

macro(boost_library_variant LIBNAME)
  set(THIS_VARIANT_COMPILE_FLAGS "${THIS_LIB_COMPILE_FLAGS}")
  set(THIS_VARIANT_LINK_FLAGS "${THIS_LIB_LINK_FLAGS}")
  set(THIS_VARIANT_LINK_LIBS "${THIS_LIB_LINK_LIBS}")

  # Determine if it is okay to build this variant
  set(THIS_VARIANT_OKAY TRUE)
  foreach(ARG ${ARGN})
    # If the library itself stated that we cannot build this variant,
    # don't. For example, we're trying to build a shared library
    # variant, but the user specified NO_SHARED in the requirements of
    # the library.
    if (THIS_LIB_NO_${ARG})
      set(THIS_VARIANT_OKAY FALSE)
    endif (THIS_LIB_NO_${ARG})

    # If the user specified that we should not build any variants of
    # this kind, don't. For example, if the BUILD_SHARED option is
    # off, don't build shared libraries.
    if(NOT BUILD_${ARG})
      set(THIS_VARIANT_OKAY FALSE)
    endif(NOT BUILD_${ARG})

    # Accumulate compile and link flags
    set(THIS_VARIANT_COMPILE_FLAGS "${THIS_VARIANT_COMPILE_FLAGS} ${THIS_LIB_${ARG}_COMPILE_FLAGS} ${${ARG}_COMPILE_FLAGS}")
    set(THIS_VARIANT_LINK_FLAGS "${THIS_VARIANT_LINK_FLAGS} ${THIS_LIB_${ARG}_LINK_FLAGS} ${${ARG}_LINK_FLAGS}")
    set(THIS_VARIANT_LINK_LIBS "${THIS_VARIANT_LINK_LIBS} ${THIS_LIB_${ARG}_LINK_LIBS} ${${ARG}_LINK_LIBS}")
  endforeach(ARG ${ARGN})

  if (THIS_VARIANT_OKAY)
    # Use the appropriate C++ flags for debug vs. release variants
    list_contains(THIS_LIB_IS_DEBUG "DEBUG" ${ARGN})
    if (THIS_LIB_IS_DEBUG)
      set(THIS_VARIANT_COMPILE_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} ${THIS_VARIANT_COMPILE_FLAGS}")
      set(THIS_VARIANT_LINK_FLAGS "${CMAKE_LINK_FLAGS_DEBUG} ${THIS_VARIANT_LINK_FLAGS}")
      set(THIS_VARIANT_LINK_LIBS "${THIS_VARIANT_LINK_LIBS}")
    else (THIS_LIB_IS_DEBUG)
      set(THIS_VARIANT_COMPILE_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} ${THIS_VARIANT_COMPILE_FLAGS}")
      set(THIS_VARIANT_LINK_FLAGS "${CMAKE_LINK_FLAGS_RELEASE} ${THIS_VARIANT_LINK_FLAGS}")
      set(THIS_VARIANT_LINK_LIBS "${THIS_VARIANT_LINK_LIBS}")
    endif (THIS_LIB_IS_DEBUG)

    # Determine the suffix for this library target
    boost_library_variant_target_name(${ARGN})
    set(VARIANT_LIBNAME "${LIBNAME}${VARIANT_TARGET_NAME}")

    # We handle static vs. dynamic libraries differently
    list_contains(THIS_LIB_IS_STATIC "STATIC" ${ARGN})
    if (THIS_LIB_IS_STATIC)
      # If the STATIC_TAG flag was set, we append "-s" to the name of
      # the library. This is an unfortunate hack, needed only for the
      # test library.
      if (THIS_LIB_STATIC_TAG)
        set(THIS_LIB_STATIC_TAG "-s")
      else(THIS_LIB_STATIC_TAG)
        set(THIS_LIB_STATIC_TAG "")
      endif(THIS_LIB_STATIC_TAG)
      
      # On Windows, we need static and shared libraries to have
      # different names, so we follow the Boost.Build version 2 style
      # and prepend "lib" to the name.
      if(WIN32 AND NOT CYGWIN)
	set(LIBPREFIX "lib")
      else(WIN32 AND NOT CYGWIN)
	set(LIBPREFIX "")
      endif(WIN32 AND NOT CYGWIN)
      
      # Add the library itself
      add_library(${VARIANT_LIBNAME} STATIC ${THIS_LIB_SOURCES})

      # Set properties on this library
      set_target_properties(${VARIANT_LIBNAME}
        PROPERTIES
        OUTPUT_NAME "${LIBPREFIX}${LIBNAME}${VARIANT_VERSIONED_NAME}${THIS_LIB_STATIC_TAG}"
        CLEAN_DIRECT_OUTPUT 1
        COMPILE_FLAGS "${THIS_VARIANT_COMPILE_FLAGS}"
        LINK_FLAGS "${THIS_VARIANT_LINK_FLAGS}"
        )
    elseif (THIS_LIB_MODULE)
      # Add a module
      add_library(${VARIANT_LIBNAME} MODULE ${THIS_LIB_SOURCES})

      # Set properties on this library
      set_target_properties(${VARIANT_LIBNAME}
        PROPERTIES
        OUTPUT_NAME "${LIBNAME}${VARIANT_VERSIONED_NAME}"
        CLEAN_DIRECT_OUTPUT 1
        COMPILE_FLAGS "${THIS_VARIANT_COMPILE_FLAGS}"
        LINK_FLAGS "${THIS_VARIANT_LINK_FLAGS}"
        SOVERSION "${BOOST_VERSION}"
        )
    else (THIS_LIB_IS_STATIC)
      # Add a module
      add_library(${VARIANT_LIBNAME} SHARED ${THIS_LIB_SOURCES})

      # Set properties on this library
      set_target_properties(${VARIANT_LIBNAME}
        PROPERTIES
        OUTPUT_NAME "${LIBNAME}${VARIANT_VERSIONED_NAME}"
        CLEAN_DIRECT_OUTPUT 1
        COMPILE_FLAGS "${THIS_VARIANT_COMPILE_FLAGS}"
        LINK_FLAGS "${THIS_VARIANT_LINK_FLAGS}"
        SOVERSION "${BOOST_VERSION}"
        )
    endif (THIS_LIB_IS_STATIC)
      
    # The basic LIBNAME target depends on each of the variants
    add_dependencies(${LIBNAME} ${VARIANT_LIBNAME})
    
    # Link against whatever libraries this library depends on
    separate_arguments(THIS_VARIANT_LINK_LIBS)
    foreach(library ${THIS_VARIANT_LINK_LIBS})
      target_link_libraries(${VARIANT_LIBNAME} ${library}) # loop maybe unnecessary here
    endforeach(library ${THIS_VARIANT_LINK_LIBS})
    foreach(dependency ${THIS_LIB_DEPENDS})
      target_link_libraries(${VARIANT_LIBNAME} "${dependency}${VARIANT_TARGET_NAME}")
    endforeach(dependency "${THIS_LIB_DEPENDS}")

    # Installation of this library variant
    install(TARGETS ${VARIANT_LIBNAME} DESTINATION lib)
  endif (THIS_VARIANT_OKAY)
endmacro(boost_library_variant)

# Updates the set of default build variants to account for variations
# in the given feature.
#
#   boost_add_default_variant(feature-val1 feature-val2 ...)
#
# Each new feature creates a new set of build variants using that
# feature. For example, writing:
# 
#    boost_add_default_variant(SINGLE_THREADED MULTI_THREADED)
#
# Will create single- and multi-threaded variants of every default
# library variant already defined, doubling the number of variants
# that will be built. See the top-level CMakeLists.txt for the set of
# default variants.
#
# Variables affected:
#
#   BOOST_DEFAULT_VARIANTS:
#     This variable describes all of the variants that will be built
#     by default, and will be updated with each invocation of
#     boost_add_default_variant. The variable itself is a list, where
#     each element in the list contains a colon-separated string
#     naming a specific set of features for that variant, e.g.,
#     STATIC:DEBUG:SINGLE_THREADED.
#
#   BOOST_ADDLIB_ARG_NAMES:
#
#     This variable describes all of the feature-specific arguments
#     that can be used for the boost_library macro, separated by
#     semicolons. For example, given the use of
#     boost_add_default_variant above, this variable will contain (at
#     least)
#
#        SINGLE_THREADED_COMPILE_FLAGS;SINGLE_THREADED_LINK_FLAGS;
#        MULTI_THREADED_COMPILE_FLAGS;MULTI_THREADED_LINK_FLAGS
#
#     When this variable is used in boost_library, it turns these
#     names into feature-specific options. For example,
#     MULTI_THREADED_COMPILE_FLAGS provides extra compile flags to be
#     used only for multi-threaded variants of the library.
#
#   BOOST_ADDLIB_OPTION_NAMES:
#
#     Like BOOST_ADDLIB_ARG_NAMES, this variable describes
#     feature-specific options to boost_library that can be used to
#     turn off building of the library when the variant would require
#     certain features. For example, the NO_SINGLE_THREADED option
#     turns off building of single-threaded variants for a library.
macro(boost_add_default_variant)
  # Update BOOST_DEFAULT_VARIANTS
  if (BOOST_DEFAULT_VARIANTS)
    set(BOOST_DEFAULT_VARIANTS_ORIG ${BOOST_DEFAULT_VARIANTS})
    set(BOOST_DEFAULT_VARIANTS)
    foreach(VARIANT ${BOOST_DEFAULT_VARIANTS_ORIG})
      foreach(FEATURE ${ARGN})
        list(APPEND BOOST_DEFAULT_VARIANTS "${VARIANT}:${FEATURE}")
      endforeach(FEATURE ${ARGN})
    endforeach(VARIANT ${BOOST_DEFAULT_VARIANTS_ORIG})
    set(BOOST_DEFAULT_VARIANTS_ORIG)
  else (BOOST_DEFAULT_VARIANTS)
    set(BOOST_DEFAULT_VARIANTS ${ARGN})
  endif (BOOST_DEFAULT_VARIANTS)

  # Feature flag options, used by the boost_library macro
  foreach(FEATURE ${ARGN})
    set(BOOST_ADDLIB_ARG_NAMES 
      "${BOOST_ADDLIB_ARG_NAMES};${FEATURE}_COMPILE_FLAGS;${FEATURE}_LINK_FLAGS:${FEATURE}_LINK_LIBS")
    set(BOOST_ADDLIB_OPTION_NAMES "${BOOST_ADDLIB_OPTION_NAMES};NO_${FEATURE}")
  endforeach(FEATURE ${ARGN})
endmacro(boost_add_default_variant)

macro(boost_library)
  parse_arguments(THIS_LIB
    "DEPENDS;LINK_LIBS;COMPILE_FLAGS;LINK_FLAGS;${BOOST_ADDLIB_ARG_NAMES}"
    "STATIC_TAG;MODULE;${BOOST_ADDLIB_OPTION_NAMES}"
    ${ARGN}
    )
  car(LIBNAME ${THIS_LIB_DEFAULT_ARGS})
  cdr(THIS_LIB_SOURCES ${THIS_LIB_DEFAULT_ARGS})

  # A top-level target that refers to all of the variants of the
  # library, collectively.
  add_custom_target(${LIBNAME})

  # Build each of the default library variants
  foreach(VARIANT_STR ${BOOST_DEFAULT_VARIANTS})
    string(REPLACE ":" ";" VARIANT ${VARIANT_STR})
    separate_arguments(VARIANT)
    boost_library_variant(${LIBNAME} ${VARIANT})
  endforeach(VARIANT_STR ${BOOST_DEFAULT_VARIANTS})
endmacro(boost_library)

