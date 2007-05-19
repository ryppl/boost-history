boost_test_run(common_factor_test LIBRARIES boost_unit_test_framework-static)
boost_test_run(octonion_test 
  SOURCES ../octonion/octonion_test.cpp
  LIBRARIES boost_unit_test_framework-static)
boost_test_run(quaternion_test 
  SOURCES ../quaternion/quaternion_test.cpp
  LIBRARIES boost_unit_test_framework-static)
boost_test_run(special_functions_test 
  SOURCES ../special_functions/special_functions_test.cpp 
  LIBRARIES boost_unit_test_framework-static)
boost_test_run(quaternion_multi_incl_test 
  SOURCES ../quaternion/quaternion_mult_incl_test.cpp 
                 ../quaternion/quaternion_mi1.cpp
                 ../quaternion/quaternion_mi2.cpp
  LIBRARIES boost_unit_test_framework-static)
boost_test_run(complex_test)
boost_test_run(hypot_test)
boost_test_run(log1p_expm1_test)
