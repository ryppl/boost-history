#if !defined(TEST_CALLABLE_VALUE_HPP)
#define TEST_CALLABLE_VALUE_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"
#include <boost/property/property_callable_value.hpp>

using namespace properties;

extern prop_callable<char> p_gl_char;
extern prop_callable<int> p_gl_int;
extern prop_callable<double const> p_gl_double_const;
extern prop_callable<bool> p_gl_bool;
extern prop_callable<int *> p_gl_pointer;
extern prop_callable<test_enum,read_tag> p_gl_enum_const;
extern prop_callable<test_pod const> p_gl_class_const;

class p_callable_value_class
  {
  
  public:
  
  p_callable_value_class();
  
  prop_callable<char> p_char;
  prop_callable<int,read_tag> p_int_const;
  prop_callable<double> p_double;
  prop_callable<bool,read_tag> p_bool_const;
  prop_callable<int * const> p_pointer_const;
  prop_callable<test_enum,read_tag> p_enum_const;
  prop_callable<test_pod> p_class;
  
  static prop_callable<char> p_st_char;
  static prop_callable<int> p_st_int;
  static prop_callable<double> p_st_double;
  static prop_callable<bool> p_st_bool;
  static prop_callable<int * const> p_st_pointer_const;
  static prop_callable<test_enum> p_st_enum;
  static prop_callable<test_pod const> p_st_class_const;
  
  };
  
#endif
