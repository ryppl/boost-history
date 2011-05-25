#if !defined(TEST_MEMBER_FUNCTION_VALUE_HPP)
#define TEST_MEMBER_FUNCTION_VALUE_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"
#include "test_member_function_impl.hpp"
#include <boost/property/property_member_function_value.hpp>

using namespace properties;

extern prop_member_function<char,p_member_function_impl_class,&p_member_function_impl_class::p_char_read,&p_member_function_impl_class::p_char_write> p_gl_char;
extern prop_member_function<int,p_member_function_impl_class,&p_member_function_impl_class::p_int_read_2,&p_member_function_impl_class::p_int_write_2> p_gl_int;
extern prop_read_member_function<double,p_member_function_impl_class,&p_member_function_impl_class::p_function_double_read> p_gl_double_const;
extern prop_write_member_function<bool,p_member_function_impl_class,&p_member_function_impl_class::p_function_bool_write> p_gl_bool_write;
extern prop_member_function<int *,p_member_function_impl_class,&p_member_function_impl_class::p_function_pointer_read,&p_member_function_impl_class::p_function_pointer_write> p_gl_pointer;
extern prop_read_member_function<test_enum,p_member_function_impl_class,&p_member_function_impl_class::p_enum_read> p_gl_enum_const;
extern prop_read_member_function<test_pod,p_member_function_impl_class,&p_member_function_impl_class::p_pod_read_2> p_gl_pod_const;

extern p_member_function_impl_class pmf_gl_char;
extern p_member_function_impl_class pmf_gl_int;
extern p_member_function_impl_class pmf_gl_double;
extern p_member_function_impl_class pmf_gl_bool;
extern p_member_function_impl_class pmf_gl_pointer;
extern p_member_function_impl_class pmf_gl_enum;
extern p_member_function_impl_class pmf_gl_pod;

class p_member_function_value_class
  {
  
  public:
  
  p_member_function_value_class();
  
  prop_member_function<char,p_member_function_impl_class,&p_member_function_impl_class::p_function_char_read,&p_member_function_impl_class::p_function_char_write> p_char;
  prop_read_member_function<int,p_member_function_impl_class,&p_member_function_impl_class::p_int_read> p_int_const;
  prop_member_function<double,p_member_function_impl_class,&p_member_function_impl_class::p_double_read,&p_member_function_impl_class::p_double_write> p_double;
  prop_read_member_function<bool,p_member_function_impl_class,&p_member_function_impl_class::p_bool_read> p_bool_const;
  prop_read_member_function<int *,p_member_function_impl_class,&p_member_function_impl_class::p_pointer_read> p_pointer_const;
  prop_member_function<test_enum,p_member_function_impl_class,&p_member_function_impl_class::p_enum_read_2,&p_member_function_impl_class::p_enum_write_2> p_enum;
  prop_read_member_function<test_pod,p_member_function_impl_class,&p_member_function_impl_class::p_function_pod_read> p_pod_const;

  static prop_write_member_function<char,p_member_function_impl_class,&p_member_function_impl_class::p_char_write_2> p_st_char;
  static prop_member_function<int,p_member_function_impl_class,&p_member_function_impl_class::p_function_int_read,&p_member_function_impl_class::p_function_int_write> p_st_int;
  static prop_member_function<double,p_member_function_impl_class,&p_member_function_impl_class::p_double_read_2,&p_member_function_impl_class::p_double_write_2> p_st_double;
  static prop_member_function<bool,p_member_function_impl_class,&p_member_function_impl_class::p_bool_read_2,&p_member_function_impl_class::p_bool_write_2> p_st_bool;
  static prop_member_function<int *,p_member_function_impl_class,&p_member_function_impl_class::p_pointer_read_2,&p_member_function_impl_class::p_pointer_write_2> p_st_pointer;
  static prop_read_member_function<test_enum,p_member_function_impl_class,&p_member_function_impl_class::p_function_enum_read> p_st_enum_const;
  static prop_write_member_function<test_pod,p_member_function_impl_class,&p_member_function_impl_class::p_pod_write> p_st_pod_write;
  
  private:
  
  p_member_function_impl_class pmf_char;
  p_member_function_impl_class pmf_int;
  p_member_function_impl_class pmf_double;
  p_member_function_impl_class pmf_bool;
  p_member_function_impl_class pmf_pointer;
  p_member_function_impl_class pmf_enum;
  p_member_function_impl_class pmf_pod;

  static p_member_function_impl_class pmf_st_char;
  static p_member_function_impl_class pmf_st_int;
  static p_member_function_impl_class pmf_st_double;
  static p_member_function_impl_class pmf_st_bool;
  static p_member_function_impl_class pmf_st_pointer;
  static p_member_function_impl_class pmf_st_enum;
  static p_member_function_impl_class pmf_st_pod;

  };
  
#endif
