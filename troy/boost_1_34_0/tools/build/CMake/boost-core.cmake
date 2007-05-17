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

IF(0)
macro(set_usage_requirements target)
  parse_arguments(USAGE_REQS
    "STATIC;SHARED"
    ""
    ${ARGN}
    )
  set_target_properties(target 
    PROPERTIES
    STATIC_USAGE_REQS "${USAGE_REQS_STATIC}"
    SHARED_USAGE_REQS "${USAGE_REQS_SHARED}"
    )
endmacro(set_usage_requirements target)
ENDIF(0)
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
  parse_arguments(THIS_SUBPROJECT
    "SRCDIRS;TESTDIRS"
    ""
    ${ARGN}
    )
  project(${libname})
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
    "NO_STATIC;NO_SHARED"
    ${ARGN}
    )
  CAR(libname ${THIS_LIB_DEFAULT_ARGS})
  CDR(sources ${THIS_LIB_DEFAULT_ARGS})

  IF(NOT "${THIS_LIB_NO_STATIC}" STREQUAL "TRUE")
    add_library("${libname}-static" STATIC ${sources})
    set_target_properties("${libname}-static" 
      # notice that the static ones have -static added to the lib name.
      # this is to accomodate those who insist on linking to the
      # static varieties even when dynamics are available.
      PROPERTIES 
      OUTPUT_NAME "${libname}-static"
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
      PROPERTIES OUTPUT_NAME "${libname}"
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

