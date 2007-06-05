boost_test_run(cstdint_test SOURCES ../cstdint_test.cpp)
boost_test_run(integer_test SOURCES ../integer_test.cpp)
boost_test_run(integer_traits_test SOURCES ../integer_traits_test.cpp
               DEPENDS boost_test_exec_monitor-static)