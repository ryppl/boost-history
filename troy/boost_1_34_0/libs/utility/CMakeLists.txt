boost_library_project(
  Utility
)

# [ run call_traits_test.cpp : -u ]  ???? -u ????

set(tests 
  addressof_test
  assert_test
  base_from_member_test
  binary_search_test
  numeric_traits_test
  shared_iterator_test
  value_init_test
  )
foreach(test ${tests})
  boost_test_run(${test})
endforeach(test ${tests})
boost_test_run(none_test SOURCES test/none_test.cpp)


# this one needs args
#boost_test_Run(compressed_pair_test DEPENDS boost_test_exec_monitor-static/<link>static : -u ]
#        [ run ../current_function_test.cpp : : : <test-info>always_show_run_output ]

#        [ run ../iterators_test.cpp ../../test/build//boost_test_exec_monitor/<link>static ]
#        [ run next_prior_test.cpp ../../test/build//boost_test_exec_monitor/<link>static ]
#        [ run ../operators_test.cpp ../../test/build//boost_test_exec_monitor/<link>static ]
#        [ run ../ref_test.cpp ../../test/build//boost_test_exec_monitor/<link>static ]

boost_test_compile_fail(noncopyable_test)


boost_test_compile(ref_ct_test)
boost_test_compile(test/result_of_test)
boost_test_compile_fail(value_init_test_fail1)
boost_test_compile_fail(value_init_test_fail2)
boost_test_compile_fail(value_init_test_fail3)
boost_test_compile_fail(checked_delete_test)
