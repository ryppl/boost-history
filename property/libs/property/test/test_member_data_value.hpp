#if !defined(TEST_MEMBER_DATA_VALUE_HPP)
#define TEST_MEMBER_DATA_VALUE_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"
#include "test_member_data_impl.hpp"
#include <boost/property/property_member_data_value.hpp>
#include <boost/property/property_notification_boost_function.hpp>

using namespace properties;

extern prop_member_data<char const,p_member_data_class,&p_member_data_class::p_data_char> p_gl_char_const;
extern prop_member_data<int,p_member_data_class,&p_member_data_class::p_data_int> p_gl_int;
extern prop_member_data<double const,p_member_data_class,&p_member_data_class::p_double_2> p_gl_double_const;
extern prop_member_data<bool,p_member_data_class,&p_member_data_class::p_bool,default_policy_tag,read_tag> p_gl_bool;
extern prop_member_data<int *,p_member_data_class,&p_member_data_class::p_data_pointer,notification_boost_function> p_gl_pointer_ntf;
extern prop_member_data<test_enum,p_member_data_class,&p_member_data_class::p_data_enum> p_gl_enum;
extern prop_member_data<test_pod,p_member_data_class,&p_member_data_class::p_data_pod,notification_boost_function> p_gl_pod_ntf;

extern p_member_data_class pmd_gl_char;
extern p_member_data_class pmd_gl_int;
extern p_member_data_class pmd_gl_double;
extern p_member_data_class pmd_gl_bool;
extern p_member_data_class pmd_gl_pointer;
extern p_member_data_class pmd_gl_enum;
extern p_member_data_class pmd_gl_pod;

class p_member_data_value_class
  {
  
  public:
  
  p_member_data_value_class();
  
  prop_member_data<char,p_member_data_class,&p_member_data_class::p_data_char> p_char;
  prop_member_data<int,p_member_data_class,&p_member_data_class::p_int_2> p_int;
  prop_member_data<double,p_member_data_class,&p_member_data_class::p_double,notification_boost_function> p_double_ntf;
  prop_member_data<bool,p_member_data_class,&p_member_data_class::p_bool_2> p_bool;
  prop_member_data<int *,p_member_data_class,&p_member_data_class::p_pointer> p_pointer;
  prop_member_data<test_enum,p_member_data_class,&p_member_data_class::p_enum_2,notification_boost_function> p_enum_ntf;
  prop_member_data<test_pod const,p_member_data_class,&p_member_data_class::p_pod_2> p_pod_const;
  
  static prop_member_data<char,p_member_data_class,&p_member_data_class::p_char> p_st_char;
  static prop_member_data<int,p_member_data_class,&p_member_data_class::p_int> p_st_int;
  static prop_member_data<double const,p_member_data_class,&p_member_data_class::p_data_double> p_st_double_const;
  static prop_member_data<bool,p_member_data_class,&p_member_data_class::p_data_bool> p_st_bool;
  static prop_member_data<int *,p_member_data_class,&p_member_data_class::p_pointer_2> p_st_pointer;
  static prop_member_data<test_enum,p_member_data_class,&p_member_data_class::p_enum,default_policy_tag,read_tag> p_st_enum_const;
  static prop_member_data<test_pod,p_member_data_class,&p_member_data_class::p_pod> p_st_pod;
  
  private:
  
  p_member_data_class pmd_char;
  p_member_data_class pmd_int;
  p_member_data_class pmd_double;
  p_member_data_class pmd_bool;
  p_member_data_class pmd_pointer;
  p_member_data_class pmd_enum;
  p_member_data_class pmd_pod;

  static p_member_data_class pmd_st_char;
  static p_member_data_class pmd_st_int;
  static p_member_data_class pmd_st_double;
  static p_member_data_class pmd_st_bool;
  static p_member_data_class pmd_st_pointer;
  static p_member_data_class pmd_st_enum;
  static p_member_data_class pmd_st_pod;

  };
  
#endif
