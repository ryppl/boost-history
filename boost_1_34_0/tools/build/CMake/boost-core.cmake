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

# Detects the kind of compiler in use. If a toolset is found,
# BOOST_VERSIONING_TOOLSET_TAG will be set to the tag string used to
# describe that toolset.
if (NOT BOOST_VERSIONING_TOOLSET_TAG)
  if (MSVC60)
    set(BOOST_VERSIONING_TOOLSET_TAG "-vc6")
  elseif(MSVC70)
    set(BOOST_VERSIONING_TOOLSET_TAG "-vc7")
  elseif(MSVC71)
    set(BOOST_VERSIONING_TOOLSET_TAG "-vc71")
  elseif(MSVC80)
    set(BOOST_VERSIONING_TOOLSET_TAG "-vc80")
  elseif(MSVC)
  elseif(BORLAND)
    set(BOOST_VERSIONING_TOOLSET_TAG "-bcb")
  elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Execute GCC with the -dumpversion option, to give us a version string
    execute_process(
      COMMAND ${CMAKE_CXX_COMPILER} "-dumpversion" 
      OUTPUT_VARIABLE GCC_VERSION_STRING)
    
    # Match only the major and minor versions of the version string
    string(REGEX MATCH "[0-9]+.[0-9]+" GCC_MAJOR_MINOR_VERSION_STRING
      "${GCC_VERSION_STRING}")
    
    # Strip out the period between the major and minor versions
    string(REGEX REPLACE "\\." "" BOOST_VERSIONING_GCC_VERSION
      "${GCC_MAJOR_MINOR_VERSION_STRING}")
    
    # Set the GCC versioning toolset
    set(BOOST_VERSIONING_TOOLSET_TAG "-gcc${BOOST_VERSIONING_GCC_VERSION}")
  endif(MSVC60)
endif (NOT BOOST_VERSIONING_TOOLSET_TAG)

# Determine whether we should use library versioning by default
if(BOOST_VERSIONING_TOOLSET_TAG)
  set(USE_VERSIONING_BY_DEFAULT ON)
else(BOOST_VERSIONING_TOOLSET_TAG)
  set(USE_VERSIONING_BY_DEFAULT OFF)
endif(BOOST_VERSIONING_TOOLSET_TAG)

option(
  USE_VERSIONING 
  "Use versioned library names, e.g., boost_filesystem-gcc41-1_34" 
  ${USE_VERSIONING_BY_DEFAULT})

if(USE_VERSIONING)
  # Set BOOST_VERSIONING_TOOLSET_TAG to the string that describes the
  # Boost.Build toolset. This is used as part of the versioned library
  # name.
  if(NOT BOOST_VERSIONING_TOOLSET_TAG)
    message("Unable to determine compiler toolset. Library versioning cannot be used")
    set(USE_VERSIONING OFF CACHE BOOL 
      "Use versioned library names, e.g., boost_filesystem-gcc41-1_34" FORCE)
    set(BOOST_LIBRARY_VERSION_STRING "")
    set(BOOST_LIBRARY_VERSION_STRING_DEBUG "")
  endif(NOT BOOST_VERSIONING_TOOLSET_TAG)
endif(USE_VERSIONING)


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
    # not require building. Always enable it.
    set(${BOOST_BUILD_LIB_OPTION} ON)
  endif (THIS_PROJECT_SRCDIRS)

  if (THIS_PROJECT_TESTDIRS)
    string(TOUPPER "TEST_BOOST_${libname_}" BOOST_TEST_LIB_OPTION)
    option(${BOOST_TEST_LIB_OPTION} 
      "Enable testing of Boost.${libname_}" 
      ON)
  endif (THIS_PROJECT_TESTDIRS)


  if(${BOOST_BUILD_LIB_OPTION})
    string(TOLOWER "${libname_}" libname)
    project(${libname})

    if(NOT EXISTS ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})
      file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})
    endif(NOT EXISTS ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})

    # currently src and testdirs are irrelevant.  At one point it seemed
    # that they would need to be kept separate and scanned in order
    # CLEANUP:  put src/test dirs back together again, if no future
    #           need for it comes up
    foreach(SUBDIR ${THIS_PROJECT_SRCDIRS} ${THIS_PROJECT_TESTDIRS})
      add_subdirectory(${SUBDIR})
    endforeach(SUBDIR ${THIS_PROJECT_SRCDIRS} ${THIS_PROJECT_TESTDIRS})
  endif(${BOOST_BUILD_LIB_OPTION})
endmacro(boost_library_project)

macro(push_back_target_property target property pushvalue)
  get_target_property(oldvalue ${target} ${property})
  if(NOT oldvalue)
    set(oldvalue "")
  endif(NOT oldvalue)
  set_target_properties(${target} PROPERTIES ${property} "${oldvalue} ${pushvalue}")
endmacro(push_back_target_property target property pushvalue)

macro(boost_library_variant_target_name)
  set(VARIANT_TARGET_NAME "")
  set(VARIANT_VERSIONED_NAME "")

  if (USE_VERSIONING)
    # If we're using versioning for the names of our generated
    # libraries, detect the full toolset name.
    set(VARIANT_VERSIONED_NAME "${BOOST_VERSIONING_TOOLSET_TAG}")
  endif (USE_VERSIONING)

  # Add -mt for multi-threaded libraries
  list_contains(variant_is_mt MULTI_THREADED ${ARGN})
  if (variant_is_mt)
    set(VARIANT_TARGET_NAME "${VARIANT_TARGET_NAME}-mt")

    if (USE_VERSIONING)
      # If we're creating versioned names, tack on "-mt"
      set(VARIANT_VERSIONED_NAME "${VARIANT_VERSIONED_NAME}-mt")
    endif (USE_VERSIONING)
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

    if (USE_VERSIONING)
      set(VARIANT_ABI_TAG "${VARIANT_ABI_TAG}d")
    endif (USE_VERSIONING)
  endif (variant_is_debug)

  if (USE_VERSIONING)
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
  endif (USE_VERSIONING)
endmacro(boost_library_variant_target_name)

macro(boost_library_variant LIBNAME)
  set(THIS_VARIANT_COMPILE_FLAGS "${THIS_LIB_COMPILE_FLAGS}")
  set(THIS_VARIANT_LINK_FLAGS "${THIS_LIB_COMPILE_FLAGS}")

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
    set(THIS_VARIANT_COMPILE_FLAGS "${THIS_VARIANT_COMPILE_FLAGS} ${THIS_LIB_${ARG}_COMPILE_FLAGS}")
    set(THIS_VARIANT_LINK_FLAGS "${THIS_VARIANT_LINK_FLAGS} ${THIS_LIB_${ARG}_LINK_FLAGS}")
  endforeach(ARG ${ARGN})

  if (THIS_VARIANT_OKAY)
    # Use the appropriate C++ flags for debug vs. release variants
    list_contains(THIS_LIB_IS_DEBUG "DEBUG" ${ARGN})
    if (THIS_LIB_IS_DEBUG)
      set(THIS_VARIANT_COMPILE_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} ${THIS_VARIANT_COMPILE_FLAGS}")
      set(THIS_VARIANT_LINK_FLAGS "${CMAKE_LINK_FLAGS_DEBUG} ${THIS_VARIANT_LINK_FLAGS}")
    else (THIS_LIB_IS_DEBUG)
      set(THIS_VARIANT_COMPILE_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} ${THIS_VARIANT_COMPILE_FLAGS}")
      set(THIS_VARIANT_LINK_FLAGS "${CMAKE_LINK_FLAGS_RELEASE} ${THIS_VARIANT_LINK_FLAGS}")
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
        )
    else (THIS_LIB_IS_STATIC)
      # Add the library itself
      add_library(${VARIANT_LIBNAME} SHARED ${THIS_LIB_SOURCES})

      # Set properties on this library
      set_target_properties(${VARIANT_LIBNAME}
        PROPERTIES
        OUTPUT_NAME "${LIBNAME}${VARIANT_VERSIONED_NAME}"
        CLEAN_DIRECT_OUTPUT 1
        COMPILE_FLAGS "${THIS_VARIANT_COMPILE_FLAGS}"
        SOVERSION "${BOOST_VERSION}"
        )
    endif (THIS_LIB_IS_STATIC)
      
    # The basic LIBNAME target depends on each of the variants
    add_dependencies(${LIBNAME} ${VARIANT_LIBNAME})
    
    # Link against whatever libraries this library depends on
    target_link_libraries(${VARIANT_LIBNAME} ${THIS_LIB_LIBRARIES})
    foreach(dependency ${THIS_LIB_DEPENDS})
      target_link_libraries(${VARIANT_LIBNAME} "${dependency}${VARIANT_TARGET_NAME}")
    endforeach(dependency "${THIS_LIB_DEPENDS}")

    # Installation of this library variant
    install(TARGETS ${VARIANT_LIBNAME} DESTINATION lib)
  endif (THIS_VARIANT_OKAY)
endmacro(boost_library_variant)

# Doug: None of this actually works, but it looks neat! I'll take
# another shot at this another time, without the attempt at macro
# recursion.
macro(boost_library_enumerate_variants LIBNAME FEATURE_OPTIONS_IN)
  message(STATUS "Feature options: ${FEATURE_OPTIONS}")

  # TODO: This is a kludge that determines when ARGN is
  # non-empty. There must be a better way!
  set(MORE_FEATURE_OPTIONS FALSE)
  foreach(ARG ${ARGN})
    set(MORE_FEATURE_OPTIONS TRUE)
  endforeach(ARG ${ARGN})

  # The options for a feature comes in as a colon-separated
  # list; split that into a real list
  string(REPLACE ":" ";" FEATURE_OPTIONS ${FEATURE_OPTIONS_IN})
  separate_arguments(FEATURE_OPTIONS)
    
  foreach (FEATURE ${FEATURE_OPTIONS})
    message(STATUS "Feature: ${FEATURE}")
    if (MORE_FEATURE_OPTIONS)
      message("Recursing...")
      # Add this feature to the variant
      list(APPEND CURRENT_VARIANT_FEATURES ${FEATURE})
      
      # Recurse to produce the full sets of features for each
      # variant
      boost_library_enumerate_variants(${LIBNAME} ${ARGN})
      
      # Remove this feature from the variant
      list(REMOVE_ITEM CURRENT_VARIANT_FEATURES ${FEATURE})
    elseif (MORE_FEATURE_OPTIONS)
      # Produce the library variant
      boost_library_variant(${LIBNAME} ${CURRENT_VARIANT_FEATURES} ${FEATURE})
    endif (MORE_FEATURE_OPTIONS)
  endforeach (FEATURE ${FEATURE_OPTIONS})
endmacro(boost_library_enumerate_variants)

macro(boost_library)
  parse_arguments(THIS_LIB
    "DEPENDS;LIBRARIES;STATIC_COMPILE_FLAGS;SHARED_COMPILE_FLAGS;DEBUG_COMPILE_FLAGS;RELEASE_COMPILE_FLAGS"
    "NO_STATIC;NO_SHARED;STATIC_TAG"
    ${ARGN}
    )
  car(LIBNAME ${THIS_LIB_DEFAULT_ARGS})
  cdr(THIS_LIB_SOURCES ${THIS_LIB_DEFAULT_ARGS})

  add_custom_target(${LIBNAME})

# See about: this doesn't work
#  set(DEFAULT_VARIANT_OPTIONS)
#  set(DEFAULT_VARIANT_OPTIONS ${DEFAULT_VARIANT_OPTIONS} "STATIC:SHARED")
#  set(DEFAULT_VARIANT_OPTIONS ${DEFAULT_VARIANT_OPTIONS} "DEBUG:RELEASE")
#  set(DEFAULT_VARIANT_OPTIONS ${DEFAULT_VARIANT_OPTIONS} "MULTI_THREADED:SINGLE_THREADED")

#  set(CURRENT_VARIANT_FEATURES)
#  boost_library_enumerate_variants (${LIBNAME} ${DEFAULT_VARIANT_OPTIONS})
  boost_library_variant(${LIBNAME} STATIC DEBUG)
  boost_library_variant(${LIBNAME} STATIC RELEASE)
  boost_library_variant(${LIBNAME} SHARED DEBUG)
  boost_library_variant(${LIBNAME} SHARED RELEASE)
endmacro(boost_library)

