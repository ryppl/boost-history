#if !defined(TEST_AUTO_VALUE_HPP)
#define TEST_AUTO_VALUE_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"
#include <boost/property/property_auto_value.hpp>
#include <boost/property/property_notification_boost_function.hpp>

using namespace properties;

extern prop_auto<char> p_gl_char;
extern prop_auto<int,default_policy_tag,read_tag> p_gl_int_const;
extern prop_auto<double> p_gl_double;
extern prop_auto<bool> p_gl_bool;
extern prop_auto<int * const> p_gl_pointer_const;
extern prop_auto<test_enum> p_gl_enum;
extern prop_auto<test_pod,notification_boost_function> p_gl_class_ntf;

class p_auto_value_class
  {
  
  public:
  
  p_auto_value_class();
  
  prop_auto<char,notification_boost_function> p_char_ntf;
  prop_auto<int> p_int;
  prop_auto<double,notification_boost_function> p_double_ntf;
  prop_auto<bool> p_bool;
  prop_auto<int *> p_pointer;
  prop_auto<test_enum,default_policy_tag,read_tag> p_enum_const;
  prop_auto<test_pod> p_class;
  
  static prop_auto<char> p_st_char;
  static prop_auto<int,default_policy_tag,read_tag> p_st_int_const;
  static prop_auto<double> p_st_double;
  static prop_auto<bool,notification_boost_function> p_st_bool_ntf;
  static prop_auto<int *> p_st_pointer;
  static prop_auto<test_enum,notification_boost_function> p_st_enum_ntf;
  static prop_auto<test_pod const> p_st_class_const;
  
  };
  
#endif
