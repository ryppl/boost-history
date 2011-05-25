#include <boost/detail/lightweight_test.hpp>
#include "test_auto_value.hpp"
#include "test_global_data.hpp"

prop_auto<char> p_gl_char;
prop_auto<int,default_policy_tag,read_tag> p_gl_int_const(34662);
prop_auto<double> p_gl_double;
prop_auto<bool> p_gl_bool;
prop_auto<int * const> p_gl_pointer_const(&gld_int);
prop_auto<test_enum> p_gl_enum(e_test_third);
prop_auto<test_pod,notification_boost_function> p_gl_pod_ntf;

p_auto_value_class::p_auto_value_class() :
  p_double_ntf(15),
  p_bool(true),
  p_enum_const(e_test_second)
  {
  }

prop_auto<char> p_auto_value_class::p_st_char('T');
prop_auto<int,default_policy_tag,read_tag> p_auto_value_class::p_st_int_const(34);
prop_auto<double> p_auto_value_class::p_st_double;
prop_auto<bool,notification_boost_function> p_auto_value_class::p_st_bool_ntf;
prop_auto<int *> p_auto_value_class::p_st_pointer;
prop_auto<test_enum,notification_boost_function> p_auto_value_class::p_st_enum_ntf;
prop_auto<test_pod const> p_auto_value_class::p_st_pod_const(test_pod('y',45,32.6,true));

int main()
  {
  
  return boost::report_errors();
  
  }

#include "test_global_data.cpp"
