#include <boost/detail/lightweight_test.hpp>
#include "test_data_value.hpp"
#include "test_global_data.hpp"

prop_data<char const,&p_data_class::p_char_2> p_gl_char_const;
prop_data<int,&gld_int> p_gl_int;
prop_data<double const,&gld_double> p_gl_double_const;
prop_data<bool,&p_data_class::p_bool,default_policy_tag,read_tag> p_gl_bool;
prop_data<int *,&gld_pointer,notification_boost_function> p_gl_pointer_ntf;
prop_data<test_enum,&p_data_class::p_enum_2,notification_boost_function> p_gl_enum_ntf;
prop_data<test_pod,&gld_pod,notification_boost_function> p_gl_pod_ntf;

p_data_value_class::p_data_value_class()
  {
  }

prop_data<char,&p_data_class::p_char> p_data_value_class::p_st_char;
prop_data<int,&p_data_class::p_int> p_data_value_class::p_st_int;
prop_data<double const,&p_data_class::p_double_2> p_data_value_class::p_st_double_const;
prop_data<bool,&gld_bool> p_data_value_class::p_st_bool;
prop_data<int *,&p_data_class::p_pointer_2> p_data_value_class::p_st_pointer;
prop_data<test_enum,&gld_enum> p_data_value_class::p_st_enum;
prop_data<test_pod const,&p_data_class::p_pod_2> p_data_value_class::p_st_pod_const;

int main()
  {
  
  return boost::report_errors();
  
  }

#include "test_global_data.cpp"
#include "test_data_impl.cpp"
