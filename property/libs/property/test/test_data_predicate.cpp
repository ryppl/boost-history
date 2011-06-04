#include <boost/test/unit_test.hpp>
#include <boost/lambda/lambda.hpp>
#include "test_data_predicate.hpp"

using namespace boost::lambda;

prop_data_predicate<char,&gld_char,notification_boost_function,predicate_failure_runtime_exception> p_gl_char_pred_ntf_err(_1 != '0');
prop_data_predicate<int,&p_data_class::p_int_2,notification_boost_function> p_gl_int_pred_ntf(_1 < 1000 && _1 > 100);
prop_data_predicate<double,&gld_double,default_policy_tag,predicate_failure_runtime_exception> p_gl_double_pred_err(_1 >= 11.0 && _1 <= 22.0);
prop_data_predicate<test_enum,&gld_enum> p_gl_enum_pred(_1 != e_test_second);

p_data_predicate_class::p_data_predicate_class() :
  p_char_pred_ntf_err(_1 == 'f' || _1 == 'g' || _1 == 'h'),
  p_int_pred(_1 >= 5000),
  p_double_pred_ntf_err(_1 <= 300.0),
  p_enum_pred_err(_1 != e_test_first)
  {
  }

prop_data_predicate<char,&p_data_class::p_char> p_data_predicate_class::p_st_char_pred(_1 != 'i');
prop_data_predicate<int,&gld_int,notification_boost_function,predicate_failure_runtime_exception> p_data_predicate_class::p_st_int_pred_ntf_err(_1 >= 35000);
prop_data_predicate<double,&p_data_class::p_double_2,default_policy_tag,predicate_failure_runtime_exception> p_data_predicate_class::p_st_double_pred_err(_1 <= 5000.0);
prop_data_predicate<test_enum,&p_data_class::p_enum_2,notification_boost_function> p_data_predicate_class::p_st_enum_pred_ntf(_1 != e_test_third);

void TestCharacter(p_data_predicate_class & tcl) 
  {
  }
  
void TestInt(p_data_predicate_class & tcl) 
  {
  }
  
void TestDouble(p_data_predicate_class & tcl) 
  {
  }
  
void TestEnum(p_data_predicate_class & tcl) 
  {
  }
  
void test_data_predicate_function()
  {
  
  p_data_predicate_class tcl;
  
  TestCharacter(tcl);
  TestInt(tcl);
  TestDouble(tcl);
  TestEnum(tcl);
  
  }

boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite* test = BOOST_TEST_SUITE( "Property Test Data Predicate Suite" );

    test->add( BOOST_TEST_CASE( &test_data_predicate_function ) );

    return test;
}

#include "test_global_data.cpp"
#include "test_data_impl.cpp"
