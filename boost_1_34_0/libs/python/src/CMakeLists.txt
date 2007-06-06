boost_library(boost_python
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
  LINK_LIBS "${PYTHON_LIBRARIES}"
  )