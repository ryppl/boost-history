boost_test_run(implicit_cast)
boost_test_compile_fail(implicit_cast_fail)
boost_test_run(../cast_test)
boost_test_run(../numeric_cast_test)
boost_test_run(../lexical_cast_test DEPENDS boost_unit_test_framework-static)


      
