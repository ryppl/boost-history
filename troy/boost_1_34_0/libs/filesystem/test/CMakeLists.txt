ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB) # -DBOOST_FILESYSTEM_STATIC_LINK)

SET(TESTS path_test operations_test 
  fstream_test convenience_test 
  large_file_support_test wide_test)

FOREACH(test ${TESTS})
  boost_test_run(${test}_static
    SOURCES ${test}.cpp
    DEPENDS boost_filesystem-static 
    COMPILE_FLAGS -DBOOST_FILESYSTEM_STATIC_LINK)
  boost_test_run(${test}_dynamic
    SOURCES ${test}.cpp
    DEPENDS boost_filesystem-shared
    COMPILE_FLAGS -DBOOST_FILESYSTEM_DYN_LINK)
ENDFOREACH(test ${TESTS})

boost_test_compile(../example/mbcopy)
boost_test_compile(../example/mbpath)
boost_test_compile(../example/simple_ls)

