boost_test_run(rational_example SOURCES ../rational_example.cpp)
boost_test_run(rational_test 
  SOURCES ../rational_test.cpp
  LIBRARIES boost_unit_test_framework-static
  )