boost_test_run(exec 
  DEPENDS boost_python-static 
  ARGS "${CMAKE_CURRENT_SOURCE_DIR}/exec.py"
  LINK_LIBS ${PYTHON_LIBRARIES})
boost_test_run(exec-dynamic 
  SOURCES exec.cpp
  ARGS "${CMAKE_CURRENT_SOURCE_DIR}/exec.py"
  DEPENDS boost_python-shared 
  LINK_LIBS ${PYTHON_LIBRARIES})