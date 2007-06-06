include(FindPythonInterp)
include(FindPythonLibs)

if (PYTHON_LIBRARIES)
  include_directories(${PYTHON_INCLUDE_PATH})
  boost_library_project(
    Python
    SRCDIRS src
    TESTDIRS test
    )
endif (PYTHON_LIBRARIES)