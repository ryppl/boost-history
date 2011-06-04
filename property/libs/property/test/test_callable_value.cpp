#include <boost/test/unit_test.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include "test_callable_value.hpp"
#include "test_function_impl.hpp"
#include "test_functor_impl.hpp"
#include "test_member_function_impl.hpp"

p_member_function_impl_class p_member_function_impl_object;
p_functor_class_double p_functor_class_double_object;
p_functor_class_pod p_functor_class_pod_object;
p_functor_class_char p_functor_class_char_object;

prop_callable<char> p_gl_char
  (
  (boost::bind(&p_member_function_impl_class::p_function_char_read,&p_member_function_impl_object)),
  (boost::bind(&p_member_function_impl_class::p_function_char_write,&p_member_function_impl_object,_1))
  );
prop_callable<int> p_gl_int(&p_function_int_read,&p_function_int_write,34662);
prop_callable<double const> p_gl_double_const(&p_function_double_read);
prop_callable<bool> p_gl_bool
  (
  (boost::bind(&p_member_function_impl_class::p_function_bool_read,&p_member_function_impl_object)),
  (boost::bind(&p_member_function_impl_class::p_function_bool_write,&p_member_function_impl_object,_1))
  );
prop_callable<int *> p_gl_pointer(&p_function_pointer_read,&p_function_pointer_write);
prop_callable<test_enum,read_tag> p_gl_enum_const((p_functor_class_enum()));
prop_callable<test_pod const> p_gl_pod_const(&p_function_pod_read);

p_callable_value_class::p_callable_value_class() :
  p_char(&p_function_char_read,&p_function_char_write),
  p_int_const(p_functor_class_int()),
  p_double(boost::ref(p_functor_class_double_object),boost::ref(p_functor_class_double_object),15),
  p_bool_const(p_functor_class_bool()),
  p_pointer_const
    (
    boost::bind(&p_member_function_impl_class::p_function_pointer_read,&p_member_function_impl_object)
    ),
  p_enum_const(&p_function_enum_read),
  p_pod(boost::ref(p_functor_class_pod_object),boost::ref(p_functor_class_pod_object),test_pod('#',997,23.6744,false))
  {
  p_iarray[0] = 56;
  p_iarray[1] = 13562;
  p_iarray[2] = 679;
  p_iarray[3] = 34;
  p_iarray[4] = 2491;
  p_iarray[5] = 856;
  p_iarray[6] = 37;
  p_iarray[7] = 932;
  p_iarray[8] = 8468;
  p_iarray[9] = 834;
  p_iarray[10] = 789;
  p_iarray[11] = 3452;
  p_iarray[12] = 741;
  p_iarray[13] = 3492;
  p_iarray[14] = 48;
  }
  
prop_callable<char> p_callable_value_class::p_st_char
  (
  boost::ref(p_functor_class_char_object),
  boost::ref(p_functor_class_char_object),
  'T'
  );
prop_callable<int> p_callable_value_class::p_st_int
  (
  (boost::bind(&p_member_function_impl_class::p_function_int_read,&p_member_function_impl_object)),
  (boost::bind(&p_member_function_impl_class::p_function_int_write,&p_member_function_impl_object,_1)),
  34
  );
prop_callable<double> p_callable_value_class::p_st_double
  (
  (boost::bind(&p_member_function_impl_class::p_function_double_read,&p_member_function_impl_object)),
  (boost::bind(&p_member_function_impl_class::p_function_double_write,&p_member_function_impl_object,_1))
  );
prop_callable<bool> p_callable_value_class::p_st_bool
  (
  &p_function_bool_read,
  &p_function_bool_write
  );
prop_callable<int * const> p_callable_value_class::p_st_pointer_const
  (
  (p_functor_class_pointer())
  );
prop_callable<test_enum> p_callable_value_class::p_st_enum
  (
  (boost::bind(&p_member_function_impl_class::p_function_enum_read,&p_member_function_impl_object)),
  (boost::bind(&p_member_function_impl_class::p_function_enum_write,&p_member_function_impl_object,_1)),
  e_test_third
  );
prop_callable<test_pod const> p_callable_value_class::p_st_pod_const
  (
  (boost::bind(&p_member_function_impl_class::p_function_pod_read,&p_member_function_impl_object))
  );
  
int p_callable_value_class::p_st_iarray[9] = {757,9,5347,369,1174,53,3456,443,6};
int p_gl_iarray[] = {7,98,347,289,14,5763,230,7783,22675,59,12};

#include <string>

void TestCharacter(p_callable_value_class & tcl) 
  {
  /*
  
  Although a character is considered an integral type, 
  doing integral arithmetic on it is really not normal.
  So we show functionality based on characters being part
  of C++ strings.
  
  */
  
  prop_callable<char> p_loc_char(&p_function_class::p_char_read,&p_function_class::p_char_write,'J');
  std::string s1("Terry");
  
  s1[0] = p_loc_char;
  
  BOOST_CHECK_EQUAL(s1,"Jerry");
  
  p_gl_char = 'k';
  
  s1[3] = p_gl_char;
  
  BOOST_CHECK_EQUAL(s1,"Jerky");
  
  p_loc_char = s1[1];
  tcl.p_char = 's';
  p_callable_value_class::p_st_char = 'o';
  
  s1[1] = p_callable_value_class::p_st_char;
  s1[2] = p_gl_char;
  s1[3] = p_loc_char;
  s1[4] = tcl.p_char;
  
  BOOST_CHECK_EQUAL(s1,"Jokes");
  
  std::string s2(1,tcl.p_char);
  
  BOOST_CHECK_EQUAL(s2,"s");
  
  p_gl_char = s1[2];
  
  BOOST_CHECK_EQUAL(p_gl_char,'k');
  
  s2 += p_callable_value_class::p_st_char;
  
  BOOST_CHECK_EQUAL(s2,"so");
  
  }
  
void TestInt(p_callable_value_class & tcl) 
  {
  }
  
void TestDouble(p_callable_value_class & tcl) 
  {
  }
  
void TestBool(p_callable_value_class & tcl) 
  {
  }
  
void TestPointer(p_callable_value_class & tcl) 
  {
  }
  
void TestEnum(p_callable_value_class & tcl) 
  {
  }
  
void TestPod(p_callable_value_class & tcl) 
  {
  }
  
void test_callable_value_function()
  {
  
  p_callable_value_class tcl;
  
  TestCharacter(tcl);
  TestInt(tcl);
  TestDouble(tcl);
  TestBool(tcl);
  TestPointer(tcl);
  TestEnum(tcl);
  TestPod(tcl);
  
  }

boost::unit_test::test_suite* init_unit_test_suite( int, char*[] )
{
    boost::unit_test::test_suite* test = BOOST_TEST_SUITE( "Property Test Callable Value Suite" );

    test->add( BOOST_TEST_CASE( &test_callable_value_function ) );

    return test;
}

#include "test_global_data.cpp"
#include "test_function_impl.cpp"
#include "test_functor_impl.cpp"
#include "test_member_function_impl.cpp"
