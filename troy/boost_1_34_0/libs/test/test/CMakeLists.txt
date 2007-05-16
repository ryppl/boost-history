add_definitions(-DBOOST_TEST_NO_AUTO_LINK=1)
#if(BUILD_SHARED_LIBS)
  add_definitions(-DBOOST_TEST_DYN_LINK=1)
#endif(BUILD_SHARED_LIBS)

if(BORLAND)
  add_definitions(-w-8080)
endif(BORLAND)

# Basic tests
#boost_test_run(class_properties_test LIBRARIES boost_unit_test_framework-static)
#boost_test_run(basic_cstring_test LIBRARIES boost_unit_test_framework-static)

# Program execution monitor tests
#boost_test_run_fail(prg_exec_fail1 LIBRARIES boost_prg_exec_monitor-static)
#boost_test_run_fail(prg_exec_fail2 LIBRARIES boost_prg_exec_monitor-static)
#boost_test_run_fail(prg_exec_fail3 LIBRARIES boost_prg_exec_monitor-static)
#boost_test_run_fail(prg_exec_fail4 LIBRARIES boost_prg_exec_monitor-static)

# Unit test framework tests
# NOTE: skipped errors_handling_test until I figure out what BBv2 is doing
boost_test_run(online_test)
boost_test_run_fail(minimal_test)
boost_test_run(foreach_test)
boost_test_run(output_test_stream_test LIBRARIES boost_unit_test_framework-shared)

# NOTE: skipped result_report_test until I figure out what BBv2 is doing

boost_test_run(parameterized_test_test LIBRARIES boost_unit_test_framework-shared)
boost_test_run(test_fp_comparisons LIBRARIES boost_unit_test_framework-shared)

# NOTE: skipped test_tools_test until I figure out what BBv2 is doing

boost_test_run(test_case_template_test LIBRARIES boost_unit_test_framework-shared)

#boost_test_run(custom_exception_test LIBRARIES boost_unit_test_framework-static)

boost_test_run(fixed_mapping_test LIBRARIES boost_unit_test_framework-shared)

# NOTE: skipped ifstream_line_iterator_test until I figure out what BBv2 is doing

# TDS: linking of boost_unit_test_framework broken at the moment

# static linking to boost_unit_test_framework-static currently broken
#boost_test_run(algorithms_test LIBRARIES boost_unit_test_framework-static)

boost_test_run(token_iterator_test LIBRARIES boost_unit_test_framework-shared)
boost_test_run(boost_check_equal_str LIBRARIES boost_unit_test_framework-shared)
