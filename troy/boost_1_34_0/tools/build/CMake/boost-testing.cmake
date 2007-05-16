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
#   these macros format the displayed name of the test as PROJECT_NAME::testname
#   where PROJECT_NAME is a global set by the cmake PROJECT macro
#   inside boost_library_subproject.  this use of a global (hrm) is standard practice 
#   in cmakeland but we might not like this...  
#
option(BUILD_TESTING "Enable testing" ON)
include(CTest)

macro(boost_test_parse_args testname)
  set(BOOST_TEST_PARSING_VAR "SOURCES")
  set(BOOST_TEST_SOURCES)
  set(BOOST_TEST_LIBRARIES)
  set(BOOST_TEST_OKAY TRUE)

  # Categorize each of the arguments
  foreach(ARG ${ARGN})
    if (${ARG} STREQUAL "SOURCES")
      set(BOOST_TEST_PARSING_VAR "SOURCES")
    elseif (${ARG} STREQUAL "LIBRARIES")
      set(BOOST_TEST_PARSING_VAR "LIBRARIES")
    else (${ARG} STREQUAL "SOURCES")
      if(${BOOST_TEST_PARSING_VAR} STREQUAL "SOURCES") 
        # Add this argument to the list of sources
        set(BOOST_TEST_SOURCES ${BOOST_TEST_SOURCES} ${ARG})
      else(${BOOST_TEST_PARSING_VAR} STREQUAL "SOURCES")
        # Add this argument to the list of libraries to link against
        set(BOOST_TEST_LIBRARIES ${BOOST_TEST_LIBRARIES} ${ARG})

        # If building static libraries is turned off..
        if (NOT BUILD_STATIC_LIBS)
          # And this is a -static library
          if (ARG MATCHES ".*-static$")
            # We cannot build this test
            set(BOOST_TEST_OKAY FALSE)
          endif (ARG MATCHES ".*-static$")
        endif (NOT BUILD_STATIC_LIBS)

        # If building shared libraries is turned off..
        if (NOT BUILD_SHARED_LIBS)
          # And this is a -shared library
          if (ARG MATCHES ".*-shared$")
            # We cannot build this test
            set(BOOST_TEST_OKAY FALSE)
          endif (ARG MATCHES ".*-shared$")
        endif (NOT BUILD_SHARED_LIBS)
      endif(${BOOST_TEST_PARSING_VAR} STREQUAL "SOURCES")
    endif (${ARG} STREQUAL "SOURCES")
  endforeach(ARG ${ARGN})

  # Determine the list of sources
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

    if (BOOST_TEST_LIBRARIES)
      target_link_libraries(${testname} ${BOOST_TEST_LIBRARIES})
    endif(BOOST_TEST_LIBRARIES)

    add_test("${PROJECT_NAME}::${testname}" ${EXECUTABLE_OUTPUT_PATH}/${testname})
  endif(BOOST_TEST_OKAY)
endmacro(boost_test_run)

macro(boost_test_run_fail testname)
  boost_test_parse_args(${testname} ${ARGN})
  if(BOOST_TEST_OKAY)
    add_executable(${testname} ${BOOST_TEST_SOURCES})

    if (BOOST_TEST_LIBRARIES)
      target_link_libraries(${testname} ${BOOST_TEST_LIBRARIES})
    endif(BOOST_TEST_LIBRARIES)

    add_test("${PROJECT_NAME}::${testname}" ${EXECUTABLE_OUTPUT_PATH}/${testname})
    set_tests_properties(${testname} PROPERTIES WILL_FAIL TRUE)
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
    set_tests_properties(${testname} PROPERTIES WILL_FAIL ON)
  endif(BOOST_TEST_OKAY)
endmacro(boost_test_compile_fail)
