#if !defined(TEST_DATA_PREDICATE_HPP)
#define TEST_DATA_PREDICATE_HPP

#include "test_enum.hpp"
#include "test_data_impl.hpp"
#include <boost/property/property_data_predicate.hpp>
#include <boost/property/property_notification_boost_function.hpp>
#include <boost/property/property_predicate_failure_runtime_exception.hpp>

using namespace properties;

extern prop_data_predicate<char,&p_data_char,notification_boost_function,predicate_failure_runtime_exception> p_gl_char_pred_ntf_err;
extern prop_data_predicate<int,&p_data_class::p_int_2,notification_boost_function> p_gl_int_pred_ntf;
extern prop_data_predicate<double,&p_data_double,default_policy_tag,predicate_failure_runtime_exception> p_gl_double_pred_err;
extern prop_data_predicate<test_enum,&p_data_enum> p_gl_enum_pred;

class p_data_predicate_class
  {
  
  public:
  
  p_data_predicate_class();
  
  prop_data_predicate<char,&p_data_class::p_char_2,notification_boost_function,predicate_failure_runtime_exception> p_char_pred_ntf_err;
  prop_data_predicate<int,&p_data_class::p_int> p_int_pred;
  prop_data_predicate<double,&p_data_class::p_double,notification_boost_function,predicate_failure_runtime_exception> p_double_pred_ntf_err;
  prop_data_predicate<test_enum,&p_data_class::p_enum,default_policy_tag,predicate_failure_runtime_exception> p_enum_pred_err;
  
  static prop_data_predicate<char,&p_data_class::p_char> p_st_char_pred;
  static prop_data_predicate<int,&p_data_int,notification_boost_function,predicate_failure_runtime_exception> p_st_int_pred_ntf_err;
  static prop_data_predicate<double,&p_data_class::p_double_2,default_policy_tag,predicate_failure_runtime_exception> p_st_double_pred_err;
  static prop_data_predicate<test_enum,&p_data_class::p_enum_2,notification_boost_function> p_st_enum_pred_ntf;
  
  };
  
#endif
