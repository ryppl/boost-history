##########################################################################
# Core Functionality for Boost                                           #
##########################################################################
# Copyright (C) 2007 Douglas Gregor <doug.gregor@gmail.com>              #
# Copyright (C) 2007 Troy Straszheim                                     #
#                                                                        #
# Distributed under the Boost Software License, Version 1.0.             #
# See accompanying file LICENSE_1_0.txt or copy at                       #
#   http://www.boost.org/LICENSE_1_0.txt                                 #
##########################################################################
# Important developer macros in this file:                               #
#                                                                        #
#   boost_library_project: Defines a Boost library project (e.g.,        #
#   Boost.Python).                                                       #
#                                                                        #
#   boost_add_library: Builds library binaries for Boost libraries       #
#   with compiled sources (e.g., boost_filesystem).                      #
##########################################################################

# Defines a Boost library project (e.g., for Boost.Python). Use as:
#
#   boost_library_project(libname
#                         [SRCDIRS srcdir1 srcdir2 ...] 
#                         [TESTDIRS testdir1 testdir2 ...])
#
# where libname is the name of the library (e.g., Python, or
# Filesystem), srcdir1, srcdir2, etc, are subdirectories containing
# library sources (for Boost libraries that build actual library
# binaries), and testdir1, testdir2, etc, are subdirectories
# containing regression tests.
#
# For libraries that build actual library binaries, this macro adds a
# option BUILD_BOOST_LIBNAME (which defaults to ON). When the option
# is ON, this macro will include the source subdirectories, and
# therefore, will build and install the library binary.
#
# For libraries that have regression tests, and when testing is
# enabled globally by the BUILD_TESTING option, this macro also
# defines the TEST_BOOST_LIBNAME option (defaults to ON). When ON, the
# generated makefiles/project files will contain regression tests for
# this library.
#
# Example: 
#   boost_library_project(
#     Thread
#     SRCDIRS src 
#     TESTDIRS test
#     )
macro(boost_library_project LIBNAME)
  parse_arguments(THIS_PROJECT
    "SRCDIRS;TESTDIRS"
    ""
    ${ARGN}
    )

  string(TOUPPER "BUILD_BOOST_${LIBNAME}" BOOST_BUILD_LIB_OPTION)
  if (THIS_PROJECT_SRCDIRS)
    # This Boost library has source directories, so provide an option
    # BUILD_BOOST_LIBNAME that allows one to turn on/off building of
    # the library.
    option(${BOOST_BUILD_LIB_OPTION} 
      "Build Boost.${LIBNAME} (prefer make targets, not this, to build individual libs)" 
      ON)
  else (THIS_PROJECT_SRCDIRS)
    # This Boost library has no source directories, and therefore does
    # not require building. Always enable it (but don't make it an
    # option in the cache).
    set(${BOOST_BUILD_LIB_OPTION} ON)
  endif (THIS_PROJECT_SRCDIRS)

  if(${BOOST_BUILD_LIB_OPTION})
    string(TOLOWER "${LIBNAME}" libname)
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
      string(TOUPPER "TEST_BOOST_${LIBNAME}" BOOST_TEST_LIB_OPTION)
      option(${BOOST_TEST_LIB_OPTION} 
        "Enable testing of Boost.${LIBNAME}" 
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

# This macro is an internal utility macro that builds the name of a
# particular variant of a library
#
#   boost_library_variant_target_name(feature1 feature2 ...)
#
# where feature1, feature2, etc. are the names of features to be
# included in this variant, e.g., MULTI_THREADED, DEBUG. 
#
# This macro sets two macros:
#   
#   VARIANT_TARGET_NAME: The suffix that should be appended to the
#   name of the library target to name this variant of the
#   library. For example, this might be "-mt-static" for a static,
#   multi-threaded variant. It should be used to name the CMake
#   library target, e.g., boost_signals-mt-static.
#
#   VARIANT_VERSIONED_NAME: The suffix that will be added to the name
#   of the generated library, containing information about the
#   particular version of the library and the toolset used to build
#   this library. For example, this might be "-gcc41-mt-1_34" for the
#   multi-threaded, release variant of the library in Boost 1.34.0 as
#   compiled with GCC 4.1.
macro(boost_library_variant_target_name)
  set(VARIANT_TARGET_NAME "")

  # The versioned name starts with the full Boost toolset
  set(VARIANT_VERSIONED_NAME "-${BOOST_TOOLSET}")

  # Add -mt for multi-threaded libraries
  list_contains(VARIANT_IS_MT MULTI_THREADED ${ARGN})
  if (VARIANT_IS_MT)
    set(VARIANT_TARGET_NAME "${VARIANT_TARGET_NAME}-mt")

    # If we're creating versioned names, tack on "-mt"
    set(VARIANT_VERSIONED_NAME "${VARIANT_VERSIONED_NAME}-mt")
  endif (VARIANT_IS_MT)

  # Add -static for static libraries, -shared for shared libraries
  list_contains(VARIANT_IS_STATIC STATIC ${ARGN})
  if (VARIANT_IS_STATIC)
    set(VARIANT_TARGET_NAME "${VARIANT_TARGET_NAME}-static")
  else (VARIANT_IS_STATIC)
    set(VARIANT_TARGET_NAME "${VARIANT_TARGET_NAME}-shared")
  endif (VARIANT_IS_STATIC)

  # Compute the ABI tag, which depends on various kinds of options
  set(VARIANT_ABI_TAG "")

  # TODO: Linking statically to the runtime library
  # TODO: Using debug versions of the standard/runtime support libs

  # Add -pydebug for debug builds of Python
  list_contains(VARIANT_IS_PYDEBUG PYTHON_DEBUG ${ARGN})
  if (VARIANT_IS_PYDEBUG)
    set(VARIANT_TARGET_NAME "${VARIANT_TARGET_NAME}-pydebug")
    set(VARIANT_ABI_TAG "${VARIANT_ABI_TAG}y")
  endif (VARIANT_IS_PYDEBUG)

  # TODO: using debug build of Python
  # TODO: STLport rather than default library
  # TODO: STLport's deprecated iostreams

  # Add -debug for debug libraries
  list_contains(VARIANT_IS_DEBUG DEBUG ${ARGN})
  if (VARIANT_IS_DEBUG)
    set(VARIANT_TARGET_NAME "${VARIANT_TARGET_NAME}-debug")
    set(VARIANT_ABI_TAG "${VARIANT_ABI_TAG}d")
  endif (VARIANT_IS_DEBUG)

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
endmacro(boost_library_variant_target_name)

# This macro is an internal utility macro that builds a particular
# variant of a boost library.
#
#   boost_library_variant(libname 
#                         feature1 feature2 ...)
#
# where libname is the name of the Boost library (e.g.,
# "boost_filesystem") and feature1, feature2, ... are the features
# that will be used in this variant. 
#
# This macro will define a new library target based on libname and the
# specific variant name (see boost_library_variant_target_name), which
# depends on the utility target libname. The compilation and linking
# flags for this library are defined by THIS_LIB_COMPILE_FLAGS,
# THIS_LIB_LINK_FLAGS, THIS_LIB_LINK_LIBS, and all of the compile and
# linking flags implied by the features provided.
#
# If any of the features listed conflict with this library, no new
# targets will be built. For example, if the library provides the
# option NOT_MULTI_THREADED, and one of the features provided is
# MULTI_THREADED, this macro will essentially be a no-op.
macro(boost_library_variant LIBNAME)
  set(THIS_VARIANT_COMPILE_FLAGS "${THIS_LIB_COMPILE_FLAGS}")
  set(THIS_VARIANT_LINK_FLAGS "${THIS_LIB_LINK_FLAGS}")
  set(THIS_VARIANT_LINK_LIBS ${THIS_LIB_LINK_LIBS})

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
    set(THIS_VARIANT_LINK_LIBS ${THIS_VARIANT_LINK_LIBS} ${THIS_LIB_${ARG}_LINK_LIBS} ${${ARG}_LINK_LIBS})
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
    target_link_libraries(${VARIANT_LIBNAME} ${THIS_VARIANT_LINK_LIBS})
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
# will create single- and multi-threaded variants of every default
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
      "${BOOST_ADDLIB_ARG_NAMES};${FEATURE}_COMPILE_FLAGS;${FEATURE}_LINK_FLAGS;${FEATURE}_LINK_LIBS")
    set(BOOST_ADDLIB_OPTION_NAMES "${BOOST_ADDLIB_OPTION_NAMES};NO_${FEATURE}")
  endforeach(FEATURE ${ARGN})
endmacro(boost_add_default_variant)

# Updates the set of "extra" build variants, which may be used to
# generate extra, library-specific variants of libraries.
#
#   boost_add_extra_variant(feature-val1 feature-val2 ...)
#
# Each extra viarant makes it possible for libraries to define extra
# variants.  For example, writing:
# 
#    boost_add_extra_variant(PYTHON_NODEBUG PYTHON_DEBUG)
#
# creates a PYTHON_NODEBUG/PYTHON_DEBUG feature pair as an extra
# variant, used by the Boost.Python library, which generates separate
# variants of the Boost.Python library: one variant uses the Python
# debug libraries, the other does not.
#
# The difference between boost_add_default_variant and
# boost_add_extra_variant is that adding a new default variant
# introduces additional variants to *all* Boost libraries, unless
# those variants are explicitly excluded by the library. Adding a new
# extra variant, on the other hand, allows librarie to specifically
# request extra variants using that feature.
#
# Variables affected:
#
#   BOOST_ADDLIB_ARG_NAMES: 
#     See boost_add_default_variant.
#
#   BOOST_ADDLIB_OPTION_NAMES:
#     See boost_add_default_variant.
macro(boost_add_extra_variant)
  foreach(FEATURE ${ARGN})
    set(BOOST_ADDLIB_ARG_NAMES 
      "${BOOST_ADDLIB_ARG_NAMES};${FEATURE}_COMPILE_FLAGS;${FEATURE}_LINK_FLAGS;${FEATURE}_LINK_LIBS")
    set(BOOST_ADDLIB_OPTION_NAMES "${BOOST_ADDLIB_OPTION_NAMES};NO_${FEATURE}")
  endforeach(FEATURE ${ARGN})  
endmacro(boost_add_extra_variant)

# Creates a new Boost library target that generates a compiled library
# (.a, .lib, .dll, .so, etc) from source files. This routine will
# actually build several different variants of the same library, with
# different compilation options, as determined by the set of "default"
# library variants.
#
#   boost_add_library(libname
#                     source1 source2 ...
#                     [COMPILE_FLAGS compileflags]
#                     [feature_COMPILE_FLAGS compileflags]
#                     [LINK_FLAGS linkflags]
#                     [feature_LINK_FLAGS linkflags]
#                     [LINK_LIBS linklibs]
#                     [feature_LINK_LIBS linklibs]
#                     [DEPENDS libdepend1 libdepend2 ...]
#                     [STATIC_TAG]
#                     [MODULE]
#                     [NOT_feature]
#                     [EXTRA_VARIANTS variant1 variant2 ...])
#
# where libname is the name of Boost library binary (e.g.,
# "boost_regex") and source1, source2, etc. are the source files used
# to build the library, e.g., cregex.cpp.
#
# This macro has a variety of options that affect its behavior. In
# several cases, we use the placeholder "feature" in the option name
# to indicate that there are actually several different kinds of
# options, each referring to a different build feature, e.g., shared
# libraries, multi-threaded, debug build, etc. For a complete listing
# of these features, please refer to the CMakeLists.txt file in the
# root of the Boost distribution, which defines the set of features
# that will be used to build Boost libraries by default.
#
# The options that affect this macro's behavior are:
#
#   COMPILE_FLAGS: Provides additional compilation flags that will be
#   used when building all variants of the library. For example, one
#   might want to add "-DBOOST_SIGNALS_NO_LIB=1" through this option
#   (which turns off auto-linking for the Signals library while
#   building it).
#
#   feature_COMPILE_FLAGS: Provides additional compilation flags that
#   will be used only when building variants of the library that
#   include the given feature. For example,
#   MULTI_THREADED_COMPILE_FLAGS are additional flags that will be
#   used when building a multi-threaded variant, while
#   SHARED_COMPILE_FLAGS will be used when building a shared library
#   (as opposed to a static library).
#
#   LINK_FLAGS: Provides additional flags that will be passed to the
#   linker when linking each variant of the library. This option
#   should not be used to link in additional libraries; see LINK_LIBS
#   and DEPENDS.
#
#   feature_LINK_FLAGS: Provides additional flags that will be passed
#   to the linker when building variants of the library that contain a
#   specific feature, e.g., MULTI_THREADED_LINK_FLAGS. This option
#   should not be used to link in additional libraries; see
#   feature_LINK_LIBS.
#
#   LINK_LIBS: Provides additional libraries against which each of the
#   library variants will be linked. For example, one might provide
#   "expat" as options to LINK_LIBS, to state that each of the library
#   variants will link against the expat library binary. Use LINK_LIBS
#   for libraries external to Boost; for Boost libraries, use DEPENDS.
#
#   feature_LINK_LIBS: Provides additional libraries for specific
#   variants of the library to link against. For example,
#   MULTI_THREADED_LINK_LIBS provides extra libraries to link into
#   multi-threaded variants of the library.
#
#   DEPENDS: States that this Boost libraries depends on and links
#   against another Boost library. The arguments to DEPENDS should be
#   the unversioned name of the Boost library, such as
#   "boost_filesystem". Like LINK_LIBS, this option states that all
#   variants of the library being built will link against the stated
#   libraries. Unlike LINK_LIBS, however, DEPENDS takes particular
#   library variants into account, always linking the variant of one
#   Boost library against the same variant of the other Boost
#   library. For example, if the boost_mpi_python library DEPENDS on
#   boost_python, multi-threaded variants of boost_mpi_python will
#   link against multi-threaded variants of boost_python.
#
#   STATIC_TAG: States that the name of static library variants on
#   Unix need to be named differently from shared library
#   variants. This particular option should only be used in rare cases
#   where the static and shared library variants are incompatible,
#   such that linking against the shared library rather than the
#   static library will cause features. When this option is provided,
#   static libraries on Unix variants will have "-s" appended to their
#   names. Note: we hope that this is a temporary solution. At
#   present, it is only used by the Test library.
#
#   MODULE: This option states that, when building a shared library,
#   the shared library should be built as a module rather than a
#   normal shared library. Modules have special meaning an behavior on
#   some platforms, such as Mac OS X.
#
#   NOT_feature: States that library variants containing a particular
#   feature should not be built. For example, passing
#   NOT_SINGLE_THREADED suppresses generation of single-threaded
#   variants of this library.
#
#   EXTRA_VARIANTS: Specifies that extra variants of this library
#   should be built, based on the features listed. Each "variant" is a 
#   colon-separated list of features. For example, passing
#     EXTRA_VARIANTS "PYTHON_NODEBUG:PYTHON_DEBUG"
#   will result in the creation of an extra set of library variants,
#   some with the PYTHON_NODEBUG feature and some with the
#   PYTHON_DEBUG feature. 
#
# Example:
#   boost_add_library(
#     boost_thread
#     barrier.cpp condition.cpp exceptions.cpp mutex.cpp once.cpp 
#     recursive_mutex.cpp thread.cpp tss_hooks.cpp tss_dll.cpp tss_pe.cpp 
#     tss.cpp xtime.cpp
#     SHARED_COMPILE_FLAGS "-DBOOST_THREAD_BUILD_DLL=1"
#     STATIC_COMPILE_FLAGS "-DBOOST_THREAD_BUILD_LIB=1"
#     NO_SINGLE_THREADED
#   )
macro(boost_add_library LIBNAME)
  parse_arguments(THIS_LIB
    "DEPENDS;COMPILE_FLAGS;LINK_FLAGS;LINK_LIBS;EXTRA_VARIANTS;${BOOST_ADDLIB_ARG_NAMES}"
    "STATIC_TAG;MODULE;${BOOST_ADDLIB_OPTION_NAMES}"
    ${ARGN}
    )
  set(THIS_LIB_SOURCES ${THIS_LIB_DEFAULT_ARGS})

  # A top-level target that refers to all of the variants of the
  # library, collectively.
  add_custom_target(${LIBNAME})

  # Build the set of variants that we will generate for this library
  set(THIS_LIB_VARIANTS)
  foreach(VARIANT ${BOOST_DEFAULT_VARIANTS})
    foreach(EXTRA_VARIANT ${THIS_LIB_EXTRA_VARIANTS})
      string(REPLACE ":" ";" FEATURES "${EXTRA_VARIANT}")
      separate_arguments(FEATURES)
      foreach(FEATURE ${FEATURES})
        list(APPEND THIS_LIB_VARIANTS "${VARIANT}:${FEATURE}")
      endforeach(FEATURE ${FEATURES})
    endforeach(EXTRA_VARIANT ${THIS_LIB_EXTRA_VARIANTS})
  endforeach(VARIANT ${BOOST_DEFAULT_VARIANTS})

  # Build each of the library variants
  foreach(VARIANT_STR ${THIS_LIB_VARIANTS})
    string(REPLACE ":" ";" VARIANT ${VARIANT_STR})
    separate_arguments(VARIANT)
    boost_library_variant(${LIBNAME} ${VARIANT})
  endforeach(VARIANT_STR ${THIS_LIB_VARIANTS})
endmacro(boost_add_library)

# TODO: Create boost_add_executable, which deals with variants well