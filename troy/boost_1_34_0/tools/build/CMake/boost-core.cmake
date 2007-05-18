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

option(
  USE_VERSIONING 
  "Use versioned library names, e.g., libboost_filesystem-gcc41-d" 
  ON)

if(USE_VERSIONING)
  # Set BOOST_VERSIONING_TOOLSET_TAG to the string that describes the
  # Boost.Build toolset. This is used as part of the versioned library
  # name.
  set(BOOST_VERSIONING_TOOLSET_TAG "")

  if (MSVC60)
    set(BOOST_VERSIONING_TOOLSET_TAG "-vc6")
  elseif(MSVC70)
    set(BOOST_VERSIONING_TOOLSET_TAG "-vc7")
  elseif(MSVC71)
    set(BOOST_VERSIONING_TOOLSET_TAG "-vc71")
  elseif(MSVC80)
    set(BOOST_VERSIONING_TOOLSET_TAG "-vc80")
  elseif(MSVC)
    message("Unknown Microsoft Visual C++ compiler: cannot version libraries")
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
  else(MSVC60)
    message("Unknown compiler: cannot version libraries")
  endif(MSVC60)

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
else(USE_VERSIONING)
  set(BOOST_LIBRARY_VERSION_STRING "")
  set(BOOST_LIBRARY_VERSION_STRING_DEBUG "")
endif(USE_VERSIONING)


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
macro(boost_library_subproject libname_)
  parse_arguments(THIS_SUBPROJECT
    "SRCDIRS;TESTDIRS"
    ""
    ${ARGN}
    )
  STRING(TOLOWER "${libname_}" libname)
  project(${libname})

  IF(NOT EXISTS ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})
    FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})
  ENDIF(NOT EXISTS ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})

  string(TOUPPER "BUILD_BOOST_${libname}" BOOST_LIB_OPTION_NAME)
  # currently src and testdirs are irrelevant.  At one point it seemed
  # that they would need to be kept separate and scanned in order
  # CLEANUP:  put src/test dirs back together again, if no future
  #           need for it comes up
  foreach(SUBDIR ${THIS_SUBPROJECT_SRCDIRS} ${THIS_SUBPROJECT_TESTDIRS})
    add_subdirectory(${SUBDIR})
  endforeach(SUBDIR ${THIS_SUBPROJECT_SRCDIRS} ${THIS_SUBPROJECT_TESTDIRS})
endmacro(boost_library_subproject)

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
    "DEPENDS;LIBRARIES;COMPILE_FLAGS;STICKY_COMPILE_FLAGS;STATIC_COMPILE_FLAGS;SHARED_COMPILE_FLAGS;STICKY_STATIC_COMPILE_FLAGS;STICKY_STATIC_LINK_FLAGS;STICKY_SHARED_COMPILE_FLAGS;STICKY_SHARED_LINK_FLAGS"
    "NO_STATIC;NO_SHARED;STATIC_TAG"
    ${ARGN}
    )
  CAR(libname ${THIS_LIB_DEFAULT_ARGS})
  CDR(sources ${THIS_LIB_DEFAULT_ARGS})

  IF(NOT "${THIS_LIB_NO_STATIC}" STREQUAL "TRUE")
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
      COMPILE_FLAGS "${THIS_LIB_COMPILE_FLAGS} ${THIS_LIB_STICKY_COMPILE_FLAGS} ${THIS_LIB_STATIC_COMPILE_FLAGS} ${THIS_LIB_STICKY_STATIC_COMPILE_FLAGS}"
      STICKY_COMPILE_FLAGS "${THIS_LIB_STICKY_COMPILE_FLAGS} ${THIS_LIB_STICKY_STATIC_COMPILE_FLAGS}"
      STICKY_LINK_FLAGS    "${THIS_LIB_STICKY_STATIC_LINK_FLAGS}"
      )
    trace("sticky statics(${libname}): ${THIS_LIB_STICKY_STATIC_COMPILE_FLAGS}")
    foreach(dependency ${THIS_LIB_DEPENDS})
      target_link_libraries("${libname}-static" "${dependency}-static")
      propagate_property(FROM_TARGET "${dependency}-static"
	FROM_PROPNAME STICKY_COMPILE_FLAGS
	TO_TARGET ${libname}-static 
	TO_PROPNAME COMPILE_FLAGS 
	)
    endforeach(dependency "${THIS_LIB_DEPENDS}")

    install(TARGETS "${libname}-static" DESTINATION lib)
  ENDIF(NOT "${THIS_LIB_NO_STATIC}" STREQUAL "TRUE")

  IF(NOT "${THIS_LIB_NO_SHARED}" STREQUAL "TRUE") 
    add_library("${libname}-shared" SHARED ${sources})
    set_target_properties("${libname}-shared" 
      PROPERTIES 
      OUTPUT_NAME "${libname}${BOOST_LIBRARY_VERSION_STRING}"
      DEBUG_OUTPUT_NAME "${libname}${BOOST_LIBRARY_VERSION_STRING_DEBUG}"
      RELWITHDEBINFO_OUTPUT_NAME "${libname}${BOOST_LIBRARY_VERSION_STRING_DEBUG}"
      CLEAN_DIRECT_OUTPUT 1
      COMPILE_FLAGS "${THIS_LIB_COMPILE_FLAGS} ${THIS_LIB_STICKY_COMPILE_FLAGS} ${THIS_LIB_SHARED_COMPILE_FLAGS} ${THIS_LIB_STICKY_SHARED_COMPILE_FLAGS}"
      STICKY_COMPILE_FLAGS "${THIS_LIB_STICKY_COMPILE_FLAGS} ${THIS_LIB_STICKY_SHARED_COMPILE_FLAGS}"
      STICKY_LINK_FLAGS    "${THIS_LIB_STICKY_SHARED_LINK_FLAGS}"
      )
    foreach(dependency ${THIS_LIB_DEPENDS})
      target_link_libraries("${libname}-shared" "${dependency}-shared")
      propagate_property(FROM_TARGET "${dependency}-shared"
	FROM_PROPNAME STICKY_COMPILE_FLAGS
	TO_TARGET ${libname}-shared
	TO_PROPNAME COMPILE_FLAGS 
	)
    endforeach(dependency ${THIS_LIB_DEPENDS})
    install(TARGETS "${libname}-shared" DESTINATION lib)
  ENDIF(NOT "${THIS_LIB_NO_SHARED}" STREQUAL "TRUE")
endmacro(boost_library)

