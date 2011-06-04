#include <boost/test/unit_test.hpp>
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

void TestCharacter(p_data_value_class & tcl) 
  {
  }
  
void TestInt(p_data_value_class & tcl) 
  {
  }
  
void TestDouble(p_data_value_class & tcl) 
  {
  }
  
void TestBool(p_data_value_class & tcl) 
  {
  }
  
void TestPointer(p_data_value_class & tcl) 
  {
  }
  
void TestEnum(p_data_value_class & tcl) 
  {
  }
  
void TestPod(p_data_value_class & tcl) 
  {
  }
  
void test_data_value_function()
  {
  
  p_data_value_class tcl;
  
  TestCharacter(tcl);
  TestInt(tcl);
  TestDouble(tcl);
  TestBool(tcl);
  TestPointer(tcl);
  TestEnum(tcl);
  TestPod(tcl);
  
  }

boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite* test = BOOST_TEST_SUITE( "Property Test Data Value Function Suite" );

    test->add( BOOST_TEST_CASE( &test_data_value_function ) );

    return test;
}

#include "test_global_data.cpp"
#include "test_data_impl.cpp"
