#include <boost/test/unit_test.hpp>
#include "test_member_function_value.hpp"
#include "test_global_data.hpp"

prop_member_function<char,p_member_function_impl_class,&p_member_function_impl_class::p_char_read,&p_member_function_impl_class::p_char_write> p_gl_char(pmf_gl_char);
prop_member_function<int,p_member_function_impl_class,&p_member_function_impl_class::p_int_read_2,&p_member_function_impl_class::p_int_write_2> p_gl_int(pmf_gl_int);
prop_read_member_function<double,p_member_function_impl_class,&p_member_function_impl_class::p_function_double_read> p_gl_double_const(pmf_gl_double);
prop_write_member_function<bool,p_member_function_impl_class,&p_member_function_impl_class::p_function_bool_write> p_gl_bool_write(pmf_gl_bool);
prop_member_function<int *,p_member_function_impl_class,&p_member_function_impl_class::p_function_pointer_read,&p_member_function_impl_class::p_function_pointer_write> p_gl_pointer(pmf_gl_pointer);
prop_read_member_function<test_enum,p_member_function_impl_class,&p_member_function_impl_class::p_enum_read> p_gl_enum_const(pmf_gl_enum);
prop_read_member_function<test_pod,p_member_function_impl_class,&p_member_function_impl_class::p_pod_read_2> p_gl_pod_const(pmf_gl_pod);

p_member_function_value_class::p_member_function_value_class() :
  p_char(pmf_char),
  p_int_const(pmf_int),
  p_double(pmf_double),
  p_bool_const(pmf_bool),
  p_pointer_const(pmf_pointer),
  p_enum(pmf_enum),
  p_pod_const(pmf_pod)
  {
  }

prop_write_member_function<char,p_member_function_impl_class,&p_member_function_impl_class::p_char_write_2> p_member_function_value_class::p_st_char(pmf_st_char);
prop_member_function<int,p_member_function_impl_class,&p_member_function_impl_class::p_function_int_read,&p_member_function_impl_class::p_function_int_write> p_member_function_value_class::p_st_int(pmf_st_int);
prop_member_function<double,p_member_function_impl_class,&p_member_function_impl_class::p_double_read_2,&p_member_function_impl_class::p_double_write_2> p_member_function_value_class::p_st_double(pmf_st_double);
prop_member_function<bool,p_member_function_impl_class,&p_member_function_impl_class::p_bool_read_2,&p_member_function_impl_class::p_bool_write_2> p_member_function_value_class::p_st_bool(pmf_st_bool);
prop_member_function<int *,p_member_function_impl_class,&p_member_function_impl_class::p_pointer_read_2,&p_member_function_impl_class::p_pointer_write_2> p_member_function_value_class::p_st_pointer(pmf_st_pointer);
prop_read_member_function<test_enum,p_member_function_impl_class,&p_member_function_impl_class::p_function_enum_read> p_member_function_value_class::p_st_enum_const(pmf_st_enum);
prop_write_member_function<test_pod,p_member_function_impl_class,&p_member_function_impl_class::p_pod_write> p_member_function_value_class::p_st_pod_write(pmf_st_pod);

void TestCharacter(p_member_function_value_class & tcl) 
  {
  }
  
void TestInt(p_member_function_value_class & tcl) 
  {
  }
  
void TestDouble(p_member_function_value_class & tcl) 
  {
  }
  
void TestBool(p_member_function_value_class & tcl) 
  {
  }
  
void TestPointer(p_member_function_value_class & tcl) 
  {
  }
  
void TestEnum(p_member_function_value_class & tcl) 
  {
  }
  
void TestPod(p_member_function_value_class & tcl) 
  {
  }
  
void test_member_function_value_function()
  {
  
  p_member_function_value_class tcl;
  
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
    boost::unit_test::test_suite* test = BOOST_TEST_SUITE( "Property Test Member Function Value Suite" );

    test->add( BOOST_TEST_CASE( &test_member_function_value_function ) );

    return test;
}

p_member_function_impl_class pmf_gl_char;
p_member_function_impl_class pmf_gl_int;
p_member_function_impl_class pmf_gl_double;
p_member_function_impl_class pmf_gl_bool;
p_member_function_impl_class pmf_gl_pointer;
p_member_function_impl_class pmf_gl_enum;
p_member_function_impl_class pmf_gl_pod;

p_member_function_impl_class p_member_function_value_class::pmf_st_char;
p_member_function_impl_class p_member_function_value_class::pmf_st_int;
p_member_function_impl_class p_member_function_value_class::pmf_st_double;
p_member_function_impl_class p_member_function_value_class::pmf_st_bool;
p_member_function_impl_class p_member_function_value_class::pmf_st_pointer;
p_member_function_impl_class p_member_function_value_class::pmf_st_enum;
p_member_function_impl_class p_member_function_value_class::pmf_st_pod;
  
#include "test_global_data.cpp"
#include "test_member_function_impl.cpp"
