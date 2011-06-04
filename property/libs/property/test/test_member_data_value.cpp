#include <boost/test/unit_test.hpp>
#include "test_member_data_value.hpp"

prop_member_data<char const,p_member_data_class,&p_member_data_class::p_data_char> p_gl_char_const(pmd_gl_char);
prop_member_data<int,p_member_data_class,&p_member_data_class::p_data_int> p_gl_int(pmd_gl_int);
prop_member_data<double const,p_member_data_class,&p_member_data_class::p_double_2> p_gl_double_const(pmd_gl_double);
prop_member_data<bool,p_member_data_class,&p_member_data_class::p_bool,default_policy_tag,read_tag> p_gl_bool(pmd_gl_bool);
prop_member_data<int *,p_member_data_class,&p_member_data_class::p_data_pointer,notification_boost_function> p_gl_pointer_ntf(pmd_gl_pointer);
prop_member_data<test_enum,p_member_data_class,&p_member_data_class::p_data_enum> p_gl_enum(pmd_gl_enum);
prop_member_data<test_pod,p_member_data_class,&p_member_data_class::p_data_pod,notification_boost_function> p_gl_pod_ntf(pmd_gl_pod);

p_member_data_value_class::p_member_data_value_class() :
  p_char(pmd_char),
  p_int(pmd_int),
  p_double_ntf(pmd_double),
  p_bool(pmd_bool),
  p_pointer(pmd_pointer),
  p_enum_ntf(pmd_enum),
  p_pod_const(pmd_pod)
  {
  }

prop_member_data<char,p_member_data_class,&p_member_data_class::p_char> p_member_data_value_class::p_st_char(pmd_st_char);
prop_member_data<int,p_member_data_class,&p_member_data_class::p_int> p_member_data_value_class::p_st_int(pmd_st_int);
prop_member_data<double const,p_member_data_class,&p_member_data_class::p_data_double> p_member_data_value_class::p_st_double_const(pmd_st_double);
prop_member_data<bool,p_member_data_class,&p_member_data_class::p_data_bool> p_member_data_value_class::p_st_bool(pmd_st_bool);
prop_member_data<int *,p_member_data_class,&p_member_data_class::p_pointer_2> p_member_data_value_class::p_st_pointer(pmd_st_pointer);
prop_member_data<test_enum,p_member_data_class,&p_member_data_class::p_enum,default_policy_tag,read_tag> p_member_data_value_class::p_st_enum_const(pmd_st_enum);
prop_member_data<test_pod,p_member_data_class,&p_member_data_class::p_pod> p_member_data_value_class::p_st_pod(pmd_st_pod);

void TestCharacter(p_member_data_value_class & tcl) 
  {
  }
  
void TestInt(p_member_data_value_class & tcl) 
  {
  }
  
void TestDouble(p_member_data_value_class & tcl) 
  {
  }
  
void TestBool(p_member_data_value_class & tcl) 
  {
  }
  
void TestPointer(p_member_data_value_class & tcl) 
  {
  }
  
void TestEnum(p_member_data_value_class & tcl) 
  {
  }
  
void TestPod(p_member_data_value_class & tcl) 
  {
  }
  
void test_member_data_value_function()
  {
  
  p_member_data_value_class tcl;
  
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
    boost::unit_test::test_suite* test = BOOST_TEST_SUITE( "Property Test Member Data Value Suite" );

    test->add( BOOST_TEST_CASE( &test_member_data_value_function ) );

    return test;
}

p_member_data_class pmd_gl_char;
p_member_data_class pmd_gl_int;
p_member_data_class pmd_gl_double;
p_member_data_class pmd_gl_bool;
p_member_data_class pmd_gl_pointer;
p_member_data_class pmd_gl_enum;
p_member_data_class pmd_gl_pod;

p_member_data_class p_member_data_value_class::pmd_st_char;
p_member_data_class p_member_data_value_class::pmd_st_int;
p_member_data_class p_member_data_value_class::pmd_st_double;
p_member_data_class p_member_data_value_class::pmd_st_bool;
p_member_data_class p_member_data_value_class::pmd_st_pointer;
p_member_data_class p_member_data_value_class::pmd_st_enum;
p_member_data_class p_member_data_value_class::pmd_st_pod;
  
#include "test_global_data.cpp"
#include "test_member_data_impl.cpp"
