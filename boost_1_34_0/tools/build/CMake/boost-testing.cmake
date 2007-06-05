##########################################################################
# Boost testing support                                                  #
##########################################################################
#
# For each of the test macros below, the first argument, testname,
# states the name of the test that will be created. If no other
# arguments are provided, the source file testname.cpp will be used as
# the source file; otherwise, all remaining arguments to the test
# macro will be used as the source files.
#
# To link in additional libraries when building the executable, pass
# LIBRARIES to the boost_*_test macro followed by followed by the list
# of libraries to link, e.g.,
#
#   boost_test_run(signal_n_test LIBRARIES boost_test_exec_monitor)
#
# There are several macros that create tests:
#
#   boost_test_run: Builds an executable and runs it as a test. The test
#                   succeeds if it builds and returns 0 when executed.
#
#   boost_test_run_fail: Builds an executable and runs it as a test. The
#                        test succeeds if it builds but returns a non-zero
#                        exit code when executed.
#  
#   boost_test_compile: Tests that the given source file compiles without 
#                       any errors.
#
#   boost_test_compile_fail: Tests that the given source file produces 
#                            errors when compiled.
#
#
#   These macros format the displayed name of the test as
#   PROJECT_NAME::testname, where PROJECT_NAME is a global set by the
#   CMake PROJECT macro inside boost_library_project.
option(BUILD_TESTING "Enable testing" OFF)
include(CTest)

macro(boost_test_parse_args testname)
  set(BOOST_TEST_OKAY TRUE)
  set(BOOST_TEST_COMPILE_FLAGS "")
  parse_arguments(BOOST_TEST 
    "SOURCES;LINK_LIBS;DEPENDS;COMPILE_FLAGS;ARGS"
    ""
    ${ARGN}
    )
    
  # Categorize each of the arguments
  foreach(ARG ${BOOST_TEST_DEPENDS})
    get_target_property(DEPEND_TYPE ${ARG} TYPE)
    get_target_property(DEPEND_LOCATION ${ARG} LOCATION)
    # If building static libraries is turned off, don't try to build
    # the test
    if (NOT BUILD_STATIC AND ${DEPEND_TYPE} STREQUAL "STATIC_LIBRARY")
      set(BOOST_TEST_OKAY FALSE)
    endif (NOT BUILD_STATIC AND ${DEPEND_TYPE} STREQUAL "STATIC_LIBRARY")

    # If building shared libraries is turned off, don't try to build
    # the test
    if (NOT BUILD_SHARED AND ${DEPEND_TYPE} STREQUAL "SHARED_LIBRARY")
      set(BOOST_TEST_OKAY FALSE)
    endif (NOT BUILD_SHARED AND ${DEPEND_TYPE} STREQUAL "SHARED_LIBRARY")
  endforeach(ARG ${BOOST_TEST_DEPENDS})

  # If no test specified, use the name of the test
  if (NOT BOOST_TEST_SOURCES)
    set(BOOST_TEST_SOURCES "${testname}.cpp")
  endif (NOT BOOST_TEST_SOURCES)

  # If testing is turned off, this test is not okay
  if (NOT BUILD_TESTING)
    set(BOOST_TEST_OKAY FALSE)
  endif(NOT BUILD_TESTING)
endmacro(boost_test_parse_args)

macro(boost_test_run testname)
  boost_test_parse_args(${testname} ${ARGN})
  if (BOOST_TEST_OKAY)
    add_executable(${testname} ${BOOST_TEST_SOURCES})
    set_target_properties(${testname}
      PROPERTIES
      COMPILE_FLAGS "${BOOST_TEST_COMPILE_FLAGS}"
      OUTPUT_NAME ${PROJECT_NAME}/${testname})
    target_link_libraries(${testname} ${BOOST_TEST_DEPENDS})
    target_link_libraries(${testname} ${BOOST_TEST_LINK_LIBS})
    add_test("${PROJECT_NAME}::${testname}" 
      ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}/${testname} ${BOOST_TEST_ARGS})
  endif(BOOST_TEST_OKAY)
endmacro(boost_test_run)

macro(boost_test_run_fail testname)
  boost_test_parse_args(${testname} ${ARGN})
  if(BOOST_TEST_OKAY)
    add_executable(${testname} ${BOOST_TEST_SOURCES})
    set_target_properties(${testname}
      PROPERTIES
      COMPILE_FLAGS "${BOOST_TEST_COMPILE_FLAGS}"
      OUTPUT_NAME ${PROJECT_NAME}/${testname})
    target_link_libraries(${testname} ${BOOST_TEST_DEPENDS})
    target_link_libraries(${testname} ${BOOST_TEST_LINK_LIBS})
    add_test("${PROJECT_NAME}::${testname}" 
      ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}/${testname} ${BOOST_TEST_ARGS})
    set_tests_properties("${PROJECT_NAME}::${testname}" 
      PROPERTIES WILL_FAIL TRUE)
  endif(BOOST_TEST_OKAY)
endmacro(boost_test_run_fail)

macro(boost_test_compile testname)
  boost_test_parse_args(${testname} ${ARGN})
  if(BOOST_TEST_OKAY)
    add_test("${PROJECT_NAME}::${testname}"
  	     ${CMAKE_CTEST_COMMAND}
             --build-and-test
             "${Boost_SOURCE_DIR}/tools/build/CMake/CompileTest"
             "${Boost_BINARY_DIR}/tools/build/CMake/CompileTest"
             --build-generator "${CMAKE_GENERATOR}"
             --build-makeprogram "${MAKEPROGRAM}"
	     --build-project CompileTest
	     --build-options -DSOURCE=${CMAKE_CURRENT_SOURCE_DIR}/${BOOST_TEST_SOURCES} -DINCLUDES=${Boost_SOURCE_DIR})
  endif(BOOST_TEST_OKAY)
endmacro(boost_test_compile)

macro(boost_test_compile_fail testname)
  boost_test_parse_args(${testname} ${ARGN})
    
  if(BOOST_TEST_OKAY)
    add_test("${PROJECT_NAME}::${testname}"
	     ${CMAKE_CTEST_COMMAND}
             --build-and-test
             "${Boost_SOURCE_DIR}/tools/build/CMake/CompileTest"
             "${Boost_BINARY_DIR}/tools/build/CMake/CompileTest"
             --build-generator "${CMAKE_GENERATOR}"
             --build-makeprogram "${MAKEPROGRAM}"
	     --build-project CompileTest
	     --build-options -DSOURCE=${CMAKE_CURRENT_SOURCE_DIR}/${BOOST_TEST_SOURCES} -DINCLUDES=${Boost_SOURCE_DIR})
    set_tests_properties("${PROJECT_NAME}::${testname}" PROPERTIES WILL_FAIL ON)
  endif(BOOST_TEST_OKAY)
endmacro(boost_test_compile_fail)
