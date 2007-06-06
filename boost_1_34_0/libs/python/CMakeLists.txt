include(FindPythonInterp)
include(FindPythonLibs)

if (PYTHON_LIBRARIES)
  include_directories(${PYTHON_INCLUDE_PATH})

  macro(boost_python_extension MODULE_NAME)
    parse_arguments(BPL_EXT 
      "" 
      "" 
      ${ARGN})

    # Create the library target itself
    add_library(${MODULE_NAME} MODULE ${BPL_EXT_DEFAULT_ARGS} )

    # Miscellaneous target properties
    set_target_properties(${MODULE_NAME} PROPERTIES PREFIX "")

    # Link against Boost.Python library
    target_link_libraries(${MODULE_NAME} boost_python-shared)

    # Link against Python libraries
    target_link_libraries(${MODULE_NAME} ${PYTHON_LIBRARIES})
  endmacro(boost_python_extension)

  boost_library_project(
    Python
    SRCDIRS src
    TESTDIRS test
    )
endif (PYTHON_LIBRARIES)