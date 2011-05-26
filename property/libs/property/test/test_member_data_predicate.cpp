#include <boost/detail/lightweight_test.hpp>
#include <boost/lambda/lambda.hpp>
#include "test_member_data_predicate.hpp"

using namespace boost::lambda;

prop_member_data_predicate<char,p_member_data_class,&p_member_data_class::p_data_char,notification_boost_function,predicate_failure_runtime_exception> p_gl_char_pred_ntf_err(_1 != 'K',pmd_gl_char);
prop_member_data_predicate<int,p_member_data_class,&p_member_data_class::p_int_2,notification_boost_function> p_gl_int_pred_ntf(_1 > 457,pmd_gl_int);
prop_member_data_predicate<double,p_member_data_class,&p_member_data_class::p_data_double,default_policy_tag,predicate_failure_runtime_exception> p_gl_double_pred_err(_1 >= 3.0 && _1 < 7.0,pmd_gl_double);
prop_member_data_predicate<test_enum,p_member_data_class,&p_member_data_class::p_data_enum> p_gl_enum_pred(_1 != e_test_first,pmd_gl_enum);

p_member_data_predicate_class::p_member_data_predicate_class() :
  p_char_pred_ntf_err(_1 == 'a' || _1 == 'b' || _1 == 'c',pmd_char),
  p_int_pred(_1 > 20,pmd_int),
  p_double_pred_ntf_err(_1 < 345.6,pmd_double),
  p_enum_pred_err(_1 != e_test_third,pmd_enum)
  {
  }

prop_member_data_predicate<char,p_member_data_class,&p_member_data_class::p_char> p_member_data_predicate_class::p_st_char_pred(_1 != 'W',pmd_st_char);
prop_member_data_predicate<int,p_member_data_class,&p_member_data_class::p_data_int,notification_boost_function,predicate_failure_runtime_exception> p_member_data_predicate_class::p_st_int_pred_ntf_err(_1 >= 40000,pmd_st_int);
prop_member_data_predicate<double,p_member_data_class,&p_member_data_class::p_double_2,default_policy_tag,predicate_failure_runtime_exception> p_member_data_predicate_class::p_st_double_pred_err(_1 <= 5.0,pmd_st_double);
prop_member_data_predicate<test_enum,p_member_data_class,&p_member_data_class::p_enum_2,notification_boost_function> p_member_data_predicate_class::p_st_enum_pred_ntf(_1 != e_test_second,pmd_st_enum);

void TestCharacter(p_member_data_predicate_class & tcl) 
  {
  }
  
void TestInt(p_member_data_predicate_class & tcl) 
  {
  }
  
void TestDouble(p_member_data_predicate_class & tcl) 
  {
  }
  
void TestEnum(p_member_data_predicate_class & tcl) 
  {
  }

int main()
  {
  
  p_member_data_predicate_class tcl;
  
  TestCharacter(tcl);
  TestInt(tcl);
  TestDouble(tcl);
  TestEnum(tcl);
  
  return boost::report_errors();
  
  }

p_member_data_class pmd_gl_char;
p_member_data_class pmd_gl_int;
p_member_data_class pmd_gl_double;
p_member_data_class pmd_gl_enum;

p_member_data_class p_member_data_predicate_class::pmd_st_char;
p_member_data_class p_member_data_predicate_class::pmd_st_int;
p_member_data_class p_member_data_predicate_class::pmd_st_double;
p_member_data_class p_member_data_predicate_class::pmd_st_enum;
  
#include "test_global_data.cpp"
#include "test_member_data_impl.cpp"
