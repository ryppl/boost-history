#include <boost/detail/lightweight_test.hpp>
#include "test_function_value.hpp"
#include "test_global_data.hpp"

prop_function<char,&p_function_class::p_char_read,&p_function_class::p_char_write> p_gl_char;
prop_function<int,&p_function_class::p_int_read_2,&p_function_class::p_int_write_2> p_gl_int;
prop_read_function<double,&p_function_double_read> p_gl_double_const;
prop_write_function<bool,&p_function_bool_write> p_gl_bool_write;
prop_function<int *,&p_function_pointer_read,&p_function_pointer_write> p_gl_pointer;
prop_read_function<test_enum,&p_function_class::p_enum_read> p_gl_enum_const;
prop_read_function<test_pod,&p_function_class::p_pod_read_2> p_gl_pod_const;

p_function_value_class::p_function_value_class()
  {
  }

prop_write_function<char,&p_function_class::p_char_write_2> p_function_value_class::p_st_char;
prop_function<int,&p_function_int_read,&p_function_int_write> p_function_value_class::p_st_int;
prop_function<double,&p_function_class::p_double_read_2,&p_function_class::p_double_write_2> p_function_value_class::p_st_double;
prop_function<bool,&p_function_class::p_bool_read_2,&p_function_class::p_bool_write_2> p_function_value_class::p_st_bool;
prop_function<int *,&p_function_class::p_pointer_read_2,&p_function_class::p_pointer_write_2> p_function_value_class::p_st_pointer;
prop_read_function<test_enum,&p_function_enum_read> p_function_value_class::p_st_enum_const;
prop_write_function<test_pod,&p_function_class::p_pod_write> p_function_value_class::p_st_pod_write;

int main()
  {
  
  return boost::report_errors();
  
  }

#include "test_global_data.cpp"
#include "test_function_impl.cpp"
