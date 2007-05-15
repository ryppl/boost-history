# Test library configuration
add_definitions(-DBOOST_TEST_NO_AUTO_LINK=1)

# Compiler-specific configuration
set(BOOST_SHARED_COMPILE_FLAGS "")
if(MSVC)
  add_definitions(/EHac)
  set(BOOST_SHARED_COMPILE_FLAGS "${BOOST_SHARED_COMPILE_FLAGS} -wd4275")
endif(MSVC)

if (BORLAND)
  add_definitions(-w-8080)
endif(BORLAND)

set(PRG_EXEC_MON_SOURCES execution_monitor.cpp cpp_main.cpp)

set(TEST_EXEC_MON_SOURCES
    compiler_log_formatter.cpp
    execution_monitor.cpp
    framework.cpp
    plain_report_formatter.cpp
    progress_monitor.cpp
    results_collector.cpp
    results_reporter.cpp
    test_main.cpp
    test_tools.cpp
    unit_test_log.cpp
    unit_test_main.cpp
    unit_test_monitor.cpp
    unit_test_parameters.cpp
    unit_test_suite.cpp
    xml_log_formatter.cpp
    xml_report_formatter.cpp)

set(UTF_SOURCES
    compiler_log_formatter.cpp
    exception_safety.cpp
    execution_monitor.cpp
    framework.cpp
    interaction_based.cpp
    logged_expectations.cpp
    plain_report_formatter.cpp
    progress_monitor.cpp
    results_collector.cpp
    results_reporter.cpp
    test_tools.cpp
    unit_test_log.cpp
    unit_test_main.cpp
    unit_test_monitor.cpp
    unit_test_parameters.cpp
    unit_test_suite.cpp
    xml_log_formatter.cpp
    xml_report_formatter.cpp)

boost_add_library(boost_prg_exec_monitor ${PRG_EXEC_MON_SOURCES}
  STATIC
  SHARED COMPILE_FLAGS "-DBOOST_TEST_DYN_LINK=1 ${BOOST_SHARED_COMPILE_FLAGS}")
boost_add_library(boost_test_exec_monitor STATIC ${TEST_EXEC_MON_SOURCES})
boost_add_library(boost_unit_test_framework ${UTF_SOURCES}
  STATIC
  SHARED COMPILE_FLAGS "-DBOOST_TEST_DYN_LINK=1 ${BOOST_SHARED_COMPILE_FLAGS}")