macro(bpl_test TESTNAME)
  parse_arguments(BPL_TEST
    "ARGS"
    ""
    ${ARGN})

  # Determine the Python and C++ source files for this test
  if (BPL_TEST_DEFAULT_ARGS)
    car(BPL_TEST_PYSOURCE ${BPL_TEST_DEFAULT_ARGS})
    cdr(BPL_TEST_SOURCES ${BPL_TEST_DEFAULT_ARGS})
  else (BPL_TEST_DEFAULT_ARGS)
    set(BPL_TEST_PYSOURCE "${TESTNAME}.py")
    set(BPL_TEST_SOURCES "${TESTNAME}.cpp")
  endif(BPL_TEST_DEFAULT_ARGS)
  set(BPL_TEST_PYSOURCE ${CMAKE_CURRENT_SOURCE_DIR}/${BPL_TEST_PYSOURCE})

  # Build the Python extension module we need
  boost_python_extension(${TESTNAME}_ext ${BPL_TEST_SOURCES})

  # Run the test itself
  add_test("${PROJECT_NAME}::${TESTNAME}"
    ${PYTHON_EXECUTABLE} ${BPL_TEST_PYSOURCE} ${BPL_TEST_ARGS})
endmacro(bpl_test)

boost_test_run(exec 
  DEPENDS boost_python-static 
  ARGS "${CMAKE_CURRENT_SOURCE_DIR}/exec.py"
  LINK_LIBS ${PYTHON_LIBRARIES})
boost_test_run(exec-dynamic 
  SOURCES exec.cpp
  ARGS "${CMAKE_CURRENT_SOURCE_DIR}/exec.py"
  DEPENDS boost_python-shared 
  LINK_LIBS ${PYTHON_LIBRARIES})

# Setup the paths we need to run Python tests
file(TO_NATIVE_PATH ${LIBRARY_OUTPUT_PATH} PYTHONPATH)
message(STATUS ${PYTHONPATH})
set(ENV{PYTHONPATH} "${PYTHONPATH}")
set(CTEST_ENVIRONMENT "PYTHONPATH=${PYTHONPATH}")

#[
#bpl-test crossmod_exception
#  : crossmod_exception.py crossmod_exception_a.cpp crossmod_exception_b.cpp
#]


bpl_test(injected)