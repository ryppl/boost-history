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

# This internal macro detects the kind of compiler in use. If a
# toolset is found, BOOST_VERSIONING_TOOLSET_TAG will be set to the
# tag string used to describe that toolset.
# 
# If WANT_FULL_INFORMATION is TRUE, then this macro will determine
# complete information. Otherwise, this macro will only determine
# whether it recognizes the toolset at all.
macro(boost_detect_toolset WANT_FULL_INFORMATION)
  set(BOOST_VERSIONING_TOOLSET_TAG)
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
    if(WANT_FULL_INFORMATION)
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
    else(WANT_FULL_INFORMATION)
      set(BOOST_VERSIONING_TOOLSET_TAG "-gcc")
    endif(WANT_FULL_INFORMATION)
  endif(MSVC60)
endmacro(boost_detect_toolset)

# Determine whether we should use library versioning by default
boost_detect_toolset(FALSE)
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
  boost_detect_toolset(TRUE)
  if(NOT BOOST_VERSIONING_TOOLSET_TAG)
    message("Unable to determine compiler toolset. Library versioning cannot be used")
    set(USE_VERSIONING OFF CACHE BOOL 
      "Use versioned library names, e.g., boost_filesystem-gcc41-1_34" FORCE)
    set(BOOST_LIBRARY_VERSION_STRING "")
    set(BOOST_LIBRARY_VERSION_STRING_DEBUG "")
  else(NOT BOOST_VERSIONING_TOOLSET_TAG)
    # TODO: Multithreading tag should go here
    
    # When determining the ABI tag, we need to differentiate between
    # what comes before the debug tag ('d') and what comes after,
    # because we can't detect at this point whether we're going to build
    # debug or not.
    set(BOOST_VERSIONING_ABI_TAG_PREFIX "")
    set(BOOST_VERSIONING_ABI_TAG_SUFFIX "")
    
    # Determine the ABI tag for versioning
    # TODO: Linking statically to C++ standard library
    # TODO: Using debug versions of the standard/runtime support libs
    # TODO: using debug build of Python
    # This is where the debug tag lives. Switch from PREFIX to SUFFIX
    # TODO: STLport rather than default library
    # TODO: STLport's deprecated iostreams
    
    set(BOOST_VERSIONING_ABI_TAG_DEBUG 
      "-${BOOST_VERSIONING_ABI_TAG_PREFIX}d${BOOST_VERSIONING_ABI_TAG_SUFFIX}")
    if(BOOST_VERSIONING_ABI_TAG_PREFIX OR BOOST_VERSIONING_ABI_TAG_SUFFIX)
      set(BOOST_VERSIONING_ABI_TAG 
        "-${BOOST_VERSIONING_ABI_TAG_PREFIX}${BOOST_VERSIONING_ABI_TAG_SUFFIX}")
    else(BOOST_VERSIONING_ABI_TAG_PREFIX OR BOOST_VERSIONING_ABI_TAG_SUFFIX)
      set(BOOST_VERSIONING_ABI_TAG "")
    endif(BOOST_VERSIONING_ABI_TAG_PREFIX OR BOOST_VERSIONING_ABI_TAG_SUFFIX)
    
    # Determine the Boost version number for versioning
    if(BOOST_VERSION_SUBMINOR GREATER 0)
      set(BOOST_VERSIONING_VERSION 
        "${BOOST_VERSION_MAJOR}_${BOOST_VERSION_MINOR}_${BOOST_VERSION_SUBMINOR}")
    else(BOOST_VERSION_SUBMINOR GREATER 0)
      set(BOOST_VERSIONING_VERSION 
        "${BOOST_VERSION_MAJOR}_${BOOST_VERSION_MINOR}")
    endif(BOOST_VERSION_SUBMINOR GREATER 0)
    
    set(BOOST_LIBRARY_VERSION_STRING
      "${BOOST_VERSIONING_TOOLSET_TAG}${BOOST_VERSIONING_ABI_TAG}-${BOOST_VERSIONING_VERSION}")
    set(BOOST_LIBRARY_VERSION_STRING_DEBUG
      "${BOOST_VERSIONING_TOOLSET_TAG}${BOOST_VERSIONING_ABI_TAG_DEBUG}-${BOOST_VERSIONING_VERSION}")
  endif(NOT BOOST_VERSIONING_TOOLSET_TAG)
else(USE_VERSIONING)
  set(BOOST_LIBRARY_VERSION_STRING "")
  set(BOOST_LIBRARY_VERSION_STRING_DEBUG "")
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

macro(propagate_property)
  trace("args:"  ${ARGN})
  parse_arguments(_
    "FROM_TARGET;TO_TARGET;FROM_PROPNAME;TO_PROPNAME"
    ""
    ${ARGN}
    )
  trace("propagate ${__FROM_TARGET}:${__FROM_PROPNAME} -> ${__TO_TARGET}:${__TO_PROPNAME} [${props_}]")
  assert(__FROM_TARGET)
  assert(__FROM_PROPNAME)
  assert(__TO_TARGET)
  get_target_property(props_ ${__FROM_TARGET} ${__FROM_PROPNAME})
  trace("::: ${props_}")
  if (props_)
    push_back_target_property(${__TO_TARGET} ${__TO_PROPNAME} ${props_})
  endif(props_)
endmacro(propagate_property)
  
macro(boost_library)
  parse_arguments(THIS_LIB
    "DEPENDS;LIBRARIES;STATIC_COMPILE_FLAGS;STATIC_COMPILE_REQUIREMENTS;STATIC_LINK_FLAGS;STATIC_LINK_REQUIREMENTS;SHARED_COMPILE_FLAGS;SHARED_COMPILE_REQUIREMENTS;SHARED_LINK_FLAGS;SHARED_LINK_REQUIREMENTS"
    "NO_STATIC;NO_SHARED;STATIC_TAG"
    ${ARGN}
    )
  CAR(libname ${THIS_LIB_DEFAULT_ARGS})
  CDR(sources ${THIS_LIB_DEFAULT_ARGS})

  ADD_CUSTOM_TARGET(${libname})

  if(NOT "${THIS_LIB_NO_STATIC}" STREQUAL "TRUE")
    # If the STATIC_TAG option was provided, we append "-s" to the end
    # of the target's name, so that it does not conflict with the
    # dynamic library.
    if (THIS_LIB_STATIC_TAG)
      set(THIS_LIB_STATIC_TAG "-s")
    else(THIS_LIB_STATIC_TAG)
      set(THIS_LIB_STATIC_TAG "")
    endif(THIS_LIB_STATIC_TAG)

    add_library("${libname}-static" STATIC ${sources})
    set_target_properties("${libname}-static" 
      # notice that the static ones have -static added to the lib name.
      # this is to accomodate those who insist on linking to the
      # static varieties even when dynamics are available.
      PROPERTIES 
      OUTPUT_NAME "${libname}${BOOST_LIBRARY_VERSION_STRING}${THIS_LIB_STATIC_TAG}"
      DEBUG_OUTPUT_NAME "${libname}${BOOST_LIBRARY_VERSION_STRING_DEBUG}${THIS_LIB_STATIC_TAG}"
      RELWITHDEBINFO_OUTPUT_NAME "${libname}${BOOST_LIBRARY_VERSION_STRING_DEBUG}${THIS_LIB_STATIC_TAG}"
      CLEAN_DIRECT_OUTPUT 1
      COMPILE_FLAGS "${THIS_LIB_STATIC_COMPILE_FLAGS}"
      COMPILE_REQUIREMENTS "${THIS_LIB_STATIC_COMPILE_REQUIREMENTS}"
      )
      
    add_dependencies(${libname} "${libname}-static")
#    trace("sticky statics(${libname}): ${THIS_LIB_STICKY_STATIC_COMPILE_FLAGS}")
    target_link_libraries("${libname}-static" ${THIS_LIB_LIBRARIES})
    foreach(dependency ${THIS_LIB_DEPENDS})
      target_link_libraries("${libname}-static" "${dependency}-static")
      propagate_property(FROM_TARGET "${dependency}-static"
	FROM_PROPNAME COMPILE_REQUIREMENTS
	TO_TARGET ${libname}-static 
	TO_PROPNAME COMPILE_FLAGS 
	)
    endforeach(dependency "${THIS_LIB_DEPENDS}")

    install(TARGETS "${libname}-static" DESTINATION lib)
  endif(NOT "${THIS_LIB_NO_STATIC}" STREQUAL "TRUE")

  if(NOT "${THIS_LIB_NO_SHARED}" STREQUAL "TRUE") 
    add_library("${libname}-shared" SHARED ${sources})
    set_target_properties("${libname}-shared" 
      PROPERTIES 
      OUTPUT_NAME "${libname}${BOOST_LIBRARY_VERSION_STRING}"
      DEBUG_OUTPUT_NAME "${libname}${BOOST_LIBRARY_VERSION_STRING_DEBUG}"
      RELWITHDEBINFO_OUTPUT_NAME "${libname}${BOOST_LIBRARY_VERSION_STRING_DEBUG}"
      CLEAN_DIRECT_OUTPUT 1
      COMPILE_FLAGS "${THIS_LIB_SHARED_COMPILE_FLAGS}"
      COMPILE_REQUIREMENTS "${THIS_LIB_SHARED_COMPILE_REQUIREMENTS}"
      SOVERSION "${BOOST_VERSION}"
      )
    add_dependencies(${libname} "${libname}-shared")
    target_link_libraries("${libname}-shared" ${THIS_LIB_LIBRARIES})
    foreach(dependency ${THIS_LIB_DEPENDS})
      target_link_libraries("${libname}-shared" "${dependency}-shared")
      propagate_property(FROM_TARGET "${dependency}-shared"
	FROM_PROPNAME COMPILE_REQUIREMENTS
	TO_TARGET ${libname}-shared
	TO_PROPNAME COMPILE_FLAGS 
	)
    endforeach(dependency ${THIS_LIB_DEPENDS})
    install(TARGETS "${libname}-shared" DESTINATION lib)
  endif(NOT "${THIS_LIB_NO_SHARED}" STREQUAL "TRUE")
endmacro(boost_library)

