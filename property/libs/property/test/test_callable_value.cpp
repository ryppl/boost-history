#include <boost/test/unit_test.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include "test_callable_value.hpp"
#include "test_function_impl.hpp"
#include "test_functor_impl.hpp"
#include "test_member_function_impl.hpp"
#include "test_global_data.hpp"

p_member_function_impl_class p_member_function_impl_object;
p_functor_class_double p_functor_class_double_object;
p_functor_class_pod p_functor_class_pod_object;
p_functor_class_char p_functor_class_char_object;

int p_gl_iarray[] = {7,98,347,289,14,5763,230,7783,22675,59,12};
int p_callable_value_class::p_st_iarray[9] = {757,9,5347,369,1174,53,3456,443,6};

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
  (boost::bind(&p_member_function_impl_class::p_function_bool_write,&p_member_function_impl_object,_1)),
  false
  );
prop_callable<int *> p_gl_pointer(&p_function_pointer_read,&p_function_pointer_write);
prop_callable<test_enum,read_tag> p_gl_enum_const((p_functor_class_enum(e_test_first)));
prop_callable<test_pod const> p_gl_pod_const(&p_function_pod_read);

p_callable_value_class::p_callable_value_class() :
  p_char(&p_function_char_read,&p_function_char_write),
  p_int_const(p_functor_class_int(8457)),
  p_double(boost::ref(p_functor_class_double_object),boost::ref(p_functor_class_double_object),15),
  p_bool_const(p_functor_class_bool(true)),
  p_pointer_const
    (
    boost::bind(&p_member_function_impl_class::p_function_pointer_read,&p_member_function_impl_object)
    ),
  p_enum_const(&p_function_enum_read),
  p_pod(boost::ref(p_functor_class_pod_object),boost::ref(p_functor_class_pod_object),test_pod('#',997,23.6744,false))
  {
  p_member_function_impl_object.st_pointer = &gld_int;
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
  (p_functor_class_pointer(&p_callable_value_class::p_st_iarray[3]))
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
  
  /*
  
  Integers can be tested with the full range of integer operations.
  
  */
  
  prop_callable<int> p_loc_int(&p_function_class::p_int_read_2,&p_function_class::p_int_write_2,563);
  
  BOOST_CHECK_EQUAL(p_gl_int,34662);
  BOOST_CHECK_EQUAL(p_callable_value_class::p_st_int,34);
  BOOST_CHECK_EQUAL(tcl.p_int_const,8457);
  p_gl_int = 44;
  p_gl_int += 12;
  BOOST_CHECK_EQUAL(p_gl_int,56);
  p_gl_int -= 16;
  BOOST_CHECK_EQUAL(p_gl_int,40);
  ++p_gl_int;
  BOOST_CHECK_EQUAL(p_gl_int,41);
  ++p_gl_int;
  ++p_gl_int;
  --p_gl_int;
  BOOST_CHECK_EQUAL(p_gl_int,42);
  p_gl_int /= 3;
  BOOST_CHECK_EQUAL(p_gl_int,14);
  p_gl_int *= 5;
  BOOST_CHECK_EQUAL(p_gl_int,70);
  
  int il1(p_loc_int++);
  BOOST_CHECK_EQUAL(il1,563);
  BOOST_CHECK_EQUAL(p_loc_int,564);
  p_loc_int >>= 2;
  BOOST_CHECK_EQUAL(p_loc_int,141);
  p_loc_int -= 81;
  p_loc_int <<= 3;
  BOOST_CHECK_EQUAL(p_loc_int,480);
  il1 = --p_loc_int;
  BOOST_CHECK_EQUAL(il1,479);
  p_loc_int -= 6;
  BOOST_CHECK_EQUAL(p_loc_int,473);
  p_loc_int |= 0x0da;
  BOOST_CHECK_EQUAL(p_loc_int,475);
  p_loc_int &= 0x076;
  BOOST_CHECK_EQUAL(p_loc_int,82);
  
  p_callable_value_class::p_st_int ^= 57;
  
  }
  
void TestDouble(p_callable_value_class & tcl) 
  {
  
  /*
  
  We can do the full range of arithmetic functionality with a double value
  
  */
  
  gld_double = 2352.346;
  
  prop_callable<double> p_loc_double(&p_function_class::p_double_read,&p_function_class::p_double_write,45.37);
  ++p_loc_double;
  BOOST_CHECK_CLOSE(p_loc_double.get(),46.37,.1);
  BOOST_CHECK_CLOSE(tcl.p_double.get(),15.0,.1);
  tcl.p_double = 2352.346;
  p_callable_value_class::p_st_double = 452.98;
  BOOST_CHECK_CLOSE(p_gl_double_const.get(),2352.346,.01);
  BOOST_CHECK_CLOSE(p_callable_value_class::p_st_double.get(),452.98,.1);
  --p_callable_value_class::p_st_double;
  BOOST_CHECK_CLOSE(p_callable_value_class::p_st_double.get(),451.98,.1);
  double d1(tcl.p_double--);
  BOOST_CHECK_CLOSE(d1,2352.346,.01);
  BOOST_CHECK_CLOSE(tcl.p_double.get(),2351.346,.01);
  tcl.p_double = 15.0;
  d1 = tcl.p_double++;
  BOOST_CHECK_CLOSE(d1,15.0,.1);
  BOOST_CHECK_CLOSE(tcl.p_double.get(),16.0,.1);
  tcl.p_double = 2351.346;
  tcl.p_double /= 36.7;
  BOOST_CHECK_CLOSE(tcl.p_double.get(),64.069373,.1);
  p_loc_double *= 756.839;
  BOOST_CHECK_CLOSE(p_loc_double.get(),35094.624,.001);
  p_callable_value_class::p_st_double -= 2497.481;
  BOOST_CHECK_CLOSE(p_callable_value_class::p_st_double.get(),-2045.501,.01);
  
  }
  
void TestBool(p_callable_value_class & tcl) 
  {
  /*
  
  Bools are integral values, 
  either 0 (false) or 1 (true) in integer arithmetic,
  but are only tested here for true or false
  
  */
  
  prop_callable<bool> p_loc_bool(&p_function_class::p_bool_read_2,&p_function_class::p_bool_write_2,true);
  
  bool b1(tcl.p_bool_const);
  
  BOOST_CHECK(b1);
  
  bool b2(p_loc_bool);
  
  BOOST_CHECK(b2);
  
  b1 = p_gl_bool;
  
  BOOST_CHECK_EQUAL(b1,false);
  
  p_callable_value_class::p_st_bool = b2;
  
  BOOST_CHECK(p_callable_value_class::p_st_bool);
  
  bool b3(p_callable_value_class::p_st_bool);
  
  BOOST_CHECK(b3);
  
  }
  
void TestPointer(p_callable_value_class & tcl) 
  {
  
  /*
  
  Pointers can be assigned, incremented, decremented,
  and take part in simple arithmetic.
  
  */
  
  prop_callable<int *> p_loc_pointer(&p_function_class::p_pointer_read,&p_function_class::p_pointer_write,&p_gl_iarray[7]);
  prop_callable<int *> p_loc_pointer2(&p_function_class::p_pointer_read_2,&p_function_class::p_pointer_write_2,&p_gl_iarray[4]);
  
  BOOST_CHECK_EQUAL(*p_loc_pointer,7783);
  
  p_loc_pointer = &p_gl_iarray[6];
  
  BOOST_CHECK_EQUAL(*p_loc_pointer,230);
  BOOST_CHECK_EQUAL(p_loc_pointer - p_loc_pointer2,2);
  
  p_loc_pointer += 4;
  BOOST_CHECK_EQUAL(*p_loc_pointer,12);
  p_loc_pointer -= 2;
  BOOST_CHECK_EQUAL(*p_loc_pointer,22675);
  
  gld_int = 489;
  BOOST_CHECK_EQUAL(*tcl.p_pointer_const,489);
  
  BOOST_CHECK_EQUAL(*p_callable_value_class::p_st_pointer_const,369);
  
  p_gl_pointer = &p_callable_value_class::p_st_iarray[5];
  
  BOOST_CHECK_EQUAL(*p_gl_pointer,53);
  p_gl_pointer -= 2;
  BOOST_CHECK_EQUAL(*p_gl_pointer,369);
  --p_gl_pointer;
  BOOST_CHECK_EQUAL(*p_gl_pointer,5347);
  p_gl_pointer += 5;
  BOOST_CHECK_EQUAL(*(p_gl_pointer--),443);
  BOOST_CHECK_EQUAL(*p_gl_pointer,3456);
  
  }
  
void TestEnum(p_callable_value_class & tcl) 
  {
  
  /*
  
  Enums are individual values. They can participate in 
  arithmetic expressions but here I will just test basic 
  usage.
  
  */
  
  prop_callable<test_enum> p_loc_enum(&p_function_class::p_enum_read_2,&p_function_class::p_enum_write_2,e_test_second);
  
  gld_enum = e_test_default;
  
  BOOST_CHECK_EQUAL(tcl.p_enum_const,e_test_default);
  BOOST_CHECK_EQUAL(p_gl_enum_const,e_test_first);
  BOOST_CHECK_EQUAL(p_loc_enum,e_test_second);
  p_loc_enum = e_test_third;
  BOOST_CHECK_EQUAL(p_loc_enum,e_test_third);
  BOOST_CHECK_EQUAL(p_callable_value_class::p_st_enum,e_test_third);
  p_callable_value_class::p_st_enum = e_test_first;
  BOOST_CHECK_EQUAL(p_callable_value_class::p_st_enum,e_test_first);
  test_enum te1(p_loc_enum);
  BOOST_CHECK_EQUAL(te1,e_test_third);
  
  te1 = p_callable_value_class::p_st_enum;
  BOOST_CHECK_EQUAL(te1,e_test_first);
  p_callable_value_class::p_st_enum = e_test_third;
  BOOST_CHECK_EQUAL(p_callable_value_class::p_st_enum,e_test_third);
  
  }
  
void TestPod(p_callable_value_class & tcl) 
  {
  
  /*
  
  Our POD class will be tested by changing fields 
  and passing to and from the properties.
  
  */
  
  prop_callable<test_pod> p_loc_pod(&p_function_class::p_pod_read_2,&p_function_class::p_pod_write_2,test_pod('^',981,4.3,false));
  
  gld_pod = test_pod('R',874,2239.457,false);
  
  BOOST_CHECK_EQUAL(p_gl_pod_const.get().an_int,874);
  BOOST_CHECK_CLOSE(p_gl_pod_const.get().a_double,2239.457,.001);
  
  p_member_function_impl_object.st_pod = test_pod('*',3209,12.356,true);
  
  BOOST_CHECK_EQUAL(p_callable_value_class::p_st_pod_const.get().an_int,3209);
  BOOST_CHECK(p_callable_value_class::p_st_pod_const.get().a_bool);
  
  BOOST_CHECK_EQUAL(p_loc_pod.get().an_int,981);
  BOOST_CHECK_EQUAL(p_loc_pod.get().a_char,'^');
  
  test_pod p1(p_callable_value_class::p_st_pod_const);
  
  BOOST_CHECK_EQUAL(p1.a_char,'*');
  BOOST_CHECK(p1.a_bool);
  
  p1 = p_loc_pod;
  p1.an_int = 991;
  p_loc_pod = p1;
  BOOST_CHECK_EQUAL(p_loc_pod.get().an_int,991);
  
  BOOST_CHECK_EQUAL(tcl.p_pod.get().an_int,997);
  BOOST_CHECK_EQUAL(tcl.p_pod.get().a_char,'#');
  
  tcl.p_pod = test_pod();
  
  BOOST_CHECK_EQUAL(tcl.p_pod.get().a_double,0);
  
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
