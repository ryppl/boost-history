set(PROGRAM_OPTIONS_LIBRARIES
  boost_program_options-static
  boost_test_exec_monitor-static)

macro(program_options_test_run TESTNAME)
  boost_test_run(${TESTNAME} 
    DEPENDS boost_program_options-static boost_test_exec_monitor-static)
  boost_test_run("${TESTNAME}_dll"
    "${TESTNAME}.cpp"
    DEPENDS boost_program_options-shared boost_test_exec_monitor-static)
endmacro(program_options_test_run)

program_options_test_run(options_description_test) 
program_options_test_run(parsers_test)
program_options_test_run(variable_map_test)
program_options_test_run(cmdline_test)
program_options_test_run(positional_options_test)
program_options_test_run(unicode_test) 
program_options_test_run(winmain) 
