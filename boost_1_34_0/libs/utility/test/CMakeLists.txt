boost_test_run(addressof_test SOURCES ../addressof_test.cpp)
boost_test_run(assert_test SOURCES ../assert_test.cpp)
boost_test_run(base_from_member_test SOURCES ../base_from_member_test.cpp)
boost_test_run(binary_search_test SOURCES ../binary_search_test.cpp)
boost_test_run(call_traits_test ARGS -u SOURCES ../call_traits_test.cpp)
boost_test_compile_fail(checked_delete_test SOURCES ../checked_delete_test.cpp)
boost_test_run(compressed_pair_test 
  SOURCES ../compressed_pair_test 
  DEPENDS boost_test_exec_monitor-static)
boost_test_run(current_function_test SOURCES ../current_function_test.cpp)
boost_test_run(iterators_test 
  SOURCES ../iterators_test.cpp 
  DEPENDS boost_test_exec_monitor-static)
boost_test_run(next_prior_test DEPENDS boost_test_exec_monitor-static)
boost_test_compile_fail(noncopyable_test SOURCES ../noncopyable_test.cpp)
boost_test_run(numeric_traits_test SOURCES ../numeric_traits_test.cpp)
if (${CMAKE_SYSTEM} MATCHES "FreeBSD-.*")
boost_test_compile_fail("operators_test_compilerbug")
elseif(${CMAKE_SYSTEM} MATCHES "FreeBSD-.*")
boost_test_run(operators_test 
  SOURCES ../operators_test.cpp
  DEPENDS boost_test_exec_monitor-static)
endif(${CMAKE_SYSTEM} MATCHES "FreeBSD-.*")
boost_test_compile(ref_ct_test SOURCES ../ref_ct_test.cpp)
boost_test_run(ref_test 
  SOURCES ../ref_test.cpp
  DEPENDS boost_test_exec_monitor-static)
boost_test_compile(result_of_test)
boost_test_run(shared_iterator_test SOURCES ../shared_iterator_test.cpp)
boost_test_run(value_init_test SOURCES ../value_init_test.cpp)
boost_test_compile_fail(value_init_test_fail1 
  SOURCES ../value_init_test_fail1.cpp)
boost_test_compile_fail(value_init_test_fail2
  SOURCES ../value_init_test_fail2.cpp)
boost_test_compile_fail(value_init_test_fail3
  SOURCES ../value_init_test_fail3.cpp)
boost_test_run(none_test)
