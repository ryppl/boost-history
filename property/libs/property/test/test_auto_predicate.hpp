#if !defined(TEST_AUTO_PREDICATE_HPP)
#define TEST_AUTO_PREDICATE_HPP

#include "test_enum.hpp"
#include <boost/property/property_auto_predicate.hpp>
#include <boost/property/property_notification_boost_function.hpp>
#include <boost/property/property_predicate_failure_runtime_exception.hpp>

using namespace properties;

extern prop_auto_predicate<char,notification_boost_function,predicate_failure_runtime_exception> p_gl_char_pred_ntf_err;
extern prop_auto_predicate<int,notification_boost_function> p_gl_int_pred_ntf;
extern prop_auto_predicate<double,default_policy_tag,predicate_failure_runtime_exception> p_gl_double_pred_err;
extern prop_auto_predicate<test_enum> p_gl_enum_pred;

class p_auto_predicate_class
  {
  
  public:
  
  p_auto_predicate_class();
  
  prop_auto_predicate<char,notification_boost_function,predicate_failure_runtime_exception> p_char_pred_ntf_err;
  prop_auto_predicate<int> p_int_pred;
  prop_auto_predicate<double,notification_boost_function,predicate_failure_runtime_exception> p_double_pred_ntf_err;
  prop_auto_predicate<test_enum,default_policy_tag,predicate_failure_runtime_exception> p_enum_pred_err;
  
  static prop_auto_predicate<char> p_st_char_pred;
  static prop_auto_predicate<int,notification_boost_function,predicate_failure_runtime_exception> p_st_int_pred_ntf_err;
  static prop_auto_predicate<double,default_policy_tag,predicate_failure_runtime_exception> p_st_double_pred_err;
  static prop_auto_predicate<test_enum,notification_boost_function> p_st_enum_pred_ntf;
  
  };
  
#endif
