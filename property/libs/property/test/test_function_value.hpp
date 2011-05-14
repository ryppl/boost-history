#if !defined(TEST_FUNCTION_VALUE_HPP)
#define TEST_FUNCTION_VALUE_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"
#include "test_function_impl.hpp"
#include <boost/property/property_function_value.hpp>

using namespace properties;

extern prop_function<char,&p_function_class::p_char_read,&p_function_class::p_char_write> p_gl_char;
extern prop_function<int,&p_function_class::p_int_read_2,&p_function_class::p_int_write_2> p_gl_int;
extern prop_read_function<double,&p_function_double_read> p_gl_double_const;
extern prop_write_function<bool,&p_function_bool_write> p_gl_bool_write;
extern prop_function<int *,&p_function_pointer_read,&p_function_pointer_write> p_gl_pointer;
extern prop_read_function<test_enum,&p_function_class::p_enum_read> p_gl_enum_const;
extern prop_read_function<test_pod,&p_function_class::p_class_read_2> p_gl_class_const;

class p_function_value_class
  {
  
  public:
  
  p_function_value_class();
  
  prop_function<char,&p_function_char_read,&p_function_char_write> p_char;
  prop_read_function<int,&p_function_class::p_int_read> p_int_const;
  prop_function<double,&p_function_class::p_double_read,&p_function_class::p_double_write> p_double;
  prop_read_function<bool,&p_function_class::p_bool_read> p_bool_const;
  prop_read_function<int *,&p_function_class::p_pointer_read> p_pointer_const;
  prop_function<test_enum,&p_function_class::p_enum_read_2,&p_function_class::p_enum_write_2> p_enum;
  prop_read_function<test_pod,&p_function_class_read> p_class_const;

  static prop_write_function<char,&p_function_class::p_char_write_2> p_st_char;
  static prop_function<int,&p_function_int_read,&p_function_int_write> p_st_int;
  static prop_function<double,&p_function_class::p_double_read_2,&p_function_class::p_double_write_2> p_st_double;
  static prop_function<bool,&p_function_class::p_bool_read_2,&p_function_class::p_bool_write_2> p_st_bool;
  static prop_read_function<int *,&p_function_class::p_pointer_read_2> p_st_pointer_const;
  static prop_read_function<test_enum,&p_function_enum_read> p_st_enum_const;
  static prop_write_function<test_pod,&p_function_class::p_class_write> p_st_class_write;
  
  };
  
#endif
