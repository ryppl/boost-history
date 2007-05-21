#TODO: Deal with these on OSF
#    <toolset>hp_cxx,<os>OSF:<cflags>"-fprm d"
 #   <toolset>gcc,<os>OSF:<cflags>-mfp-rounding-mode=d

boost_test_compile(integer)

boost_test_run(add)
boost_test_run(det)
boost_test_run(fmod)
boost_test_run(mul)
boost_test_run(overflow)
boost_test_run(pi)
boost_test_run(pow)

boost_test_run(cmp LIBRARIES boost_test_exec_monitor-static)
boost_test_run(cmp_exn LIBRARIES boost_test_exec_monitor-static)
boost_test_run(cmp_exp LIBRARIES boost_test_exec_monitor-static)
boost_test_run(cmp_lex LIBRARIES boost_test_exec_monitor-static)
boost_test_run(cmp_set LIBRARIES boost_test_exec_monitor-static)
boost_test_run(cmp_tribool LIBRARIES boost_test_exec_monitor-static)
boost_test_run(test_float LIBRARIES boost_test_exec_monitor-static)
