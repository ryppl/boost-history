file(GLOB TEST_FILES "*.cpp")
foreach(TEST_FILE ${TEST_FILES})
  if (TEST_FILE MATCHES "odr.*cpp")
    # Skip ODR tests; we'll handle them separately
  else (TEST_FILE MATCHES "odr.*cpp")
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    boost_test_compile("${TEST_NAME}_native" 
      SOURCES ${TEST_FILE} COMPILE_FLAGS "-DBOOST_TYPEOF_NATIVE")
    boost_test_compile("${TEST_NAME}_emulation" 
      SOURCES ${TEST_FILE} COMPILE_FLAGS "-DBOOST_TYPEOF_EMULATION")
  endif (TEST_FILE MATCHES "odr.*cpp")
endforeach(TEST_FILE ${TEST_FILES})

boost_test_run(odr_native 
  SOURCES odr1.cpp odr2.cpp COMPILE_FLAGS "-DBOOST_TYPEOF_NATIVE")
boost_test_run(odr_emulation
  SOURCES odr1.cpp odr2.cpp COMPILE_FLAGS "-DBOOST_TYPEOF_EMULATION")
boost_test_run(odr_no_uns
  SOURCES odr_no_uns1.cpp odr_no_uns2.cpp 
  COMPILE_FLAGS "-DBOOST_TYPEOF_EMULATION")
  