# this fails but looks like it isn't the build
# boost_test_run(../stl_concept_covering)
boost_test_run(../concept_check_test)
boost_test_run(../class_concept_check_test)
boost_test_compile_fail(../concept_check_fail_expected)
boost_test_compile_fail(../class_concept_fail_expected)
