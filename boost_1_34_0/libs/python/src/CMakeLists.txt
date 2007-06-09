if (PYTHON_DEBUG_LIBRARIES AND BUILD_BOOST_PYTHON)
  option(BUILD_PYTHON_DEBUG
    "Build an additional Boost.Python library with Python debugging enabled"
    OFF)
endif (PYTHON_DEBUG_LIBRARIES AND BUILD_BOOST_PYTHON)

# Always build the non-debug variants of the boost_python library
set(BUILD_PYTHON_NODEBUG ON)

boost_add_library(boost_python
  numeric.cpp 
  list.cpp 
  long.cpp 
  dict.cpp 
  tuple.cpp 
  str.cpp 
  slice.cpp
  aix_init_module.cpp 
  converter/from_python.cpp 
  converter/registry.cpp
  converter/type_id.cpp
  object/enum.cpp
  object/class.cpp
  object/function.cpp
  object/inheritance.cpp
  object/life_support.cpp
  object/pickle_support.cpp
  errors.cpp
  module.cpp
  converter/builtin_converters.cpp
  converter/arg_to_python_base.cpp
  object/iterator.cpp
  object/stl_iterator.cpp
  object_protocol.cpp
  object_operators.cpp
  wrapper.cpp
  import.cpp
  exec.cpp
  STATIC_COMPILE_FLAGS "-DBOOST_PYTHON_SOURCE -DBOOST_PYTHON_STATIC_LIB"
  SHARED_COMPILE_FLAGS "-DBOOST_PYTHON_SOURCE"
  PYTHON_NODEBUG_LINK_LIBS "${PYTHON_LIBRARIES}"

  # Support for Python debugging
  EXTRA_VARIANTS PYTHON_NODEBUG:PYTHON_DEBUG
  PYTHON_DEBUG_COMPILE_FLAGS "-DBOOST_DEBUG_PYTHON -DPy_DEBUG"
  PYTHON_DEBUG_LINK_LIBS "${PYTHON_DEBUG_LIBRARIES}"
  )
