#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "test_auto_value.hpp"
#include "test_global_data.hpp"

prop_auto<char> p_gl_char;
prop_auto<int,default_policy_tag,read_tag> p_gl_int_const(34662);
prop_auto<double> p_gl_double;
prop_auto<bool> p_gl_bool;
prop_auto<int * const> p_gl_pointer_const(&gld_int);
prop_auto<test_enum> p_gl_enum(e_test_third);
prop_auto<test_pod,notification_boost_function> p_gl_pod_ntf;

p_auto_value_class::p_auto_value_class() :
  p_double_ntf(15),
  p_bool(true),
  p_pointer(&p_iarray[3]),
  p_enum_const(e_test_second),
  p_pod(test_pod('#',997,23.6744,false))
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

prop_auto<char> p_auto_value_class::p_st_char('T');
prop_auto<int,default_policy_tag,read_tag> p_auto_value_class::p_st_int_const(34);
prop_auto<double> p_auto_value_class::p_st_double;
prop_auto<bool,notification_boost_function> p_auto_value_class::p_st_bool_ntf;
prop_auto<int *> p_auto_value_class::p_st_pointer(&p_st_iarray[5]);
prop_auto<test_enum,notification_boost_function> p_auto_value_class::p_st_enum_ntf(e_test_third);
prop_auto<test_pod const> p_auto_value_class::p_st_pod_const(test_pod('y',45,32.6,true));

int p_auto_value_class::p_st_iarray[9] = {757,9,5347,369,1174,53,3456,443,6};
int p_gl_iarray[] = {7,98,347,289,14,5763,230,7783,22675,59,12};

bool TestCharacterNotificationWorked(false);

void TestCharacterNotification(const i_property_write<char> &,boost::optional<char>,char cnew)
  {
  BOOST_CHECK_EQUAL(cnew,'i');
  TestCharacterNotificationWorked = true;
  }

bool TestBoolNotificationWorked(false);

void TestBoolNotification(const i_property_write<bool> &,boost::optional<bool>,bool cnew)
  {
  BOOST_CHECK(cnew);
  TestBoolNotificationWorked = true;
  }

bool TestPointerNotificationWorked(false);

void TestPointerNotification(const i_property_write<int *> &,boost::optional<int *>,int *)
  {
  TestPointerNotificationWorked = true;
  }

bool TestEnumNotificationWorked(false);

void TestEnumNotification(const i_property_write<test_enum> &,boost::optional<test_enum>,test_enum cnew)
  {
  BOOST_CHECK_EQUAL(cnew,e_test_third);
  TestEnumNotificationWorked = true;
  }

bool TestPodNotificationWorked(false);

void TestPodNotification(const i_property_write<test_pod> &,boost::optional<test_pod>,test_pod cnew)
  {
  BOOST_CHECK_EQUAL(cnew.an_int,547);
  TestPodNotificationWorked = true;
  }

bool TestIntegerNotificationWorked(false);

void TestIntegerNotification(const i_property_write<int> &,boost::optional<int>,int cnew)
  {
  BOOST_CHECK_EQUAL(cnew,23611);
  TestIntegerNotificationWorked = true;
  }

bool TestDoubleNotificationWorked(false);

void TestDoubleNotification(const i_property_write<double> &,boost::optional<double>,double cnew)
  {
  BOOST_CHECK_CLOSE(cnew,566655.298,.0001);
  TestDoubleNotificationWorked = true;
  }

#include <string>

void TestCharacter(p_auto_value_class & tcl) 
  {
  
  /*
  
  Although a character is considered an integral type, 
  doing integral arithmetic on it is really not normal.
  So we show functionality based on characters being part
  of C++ strings.
  
  */
  
  prop_auto<char> p_loc_char('J');
  std::string s1("Terry");
  
  s1[0] = p_loc_char;
  
  BOOST_CHECK_EQUAL(s1,"Jerry");
  
  p_gl_char = 'k';
  
  s1[3] = p_gl_char;
  
  BOOST_CHECK_EQUAL(s1,"Jerky");
  
  p_loc_char = s1[1];
  tcl.p_char_ntf = 's';
  p_auto_value_class::p_st_char = 'o';
  
  s1[1] = p_auto_value_class::p_st_char;
  s1[2] = p_gl_char;
  s1[3] = p_loc_char;
  s1[4] = tcl.p_char_ntf;
  
  BOOST_CHECK_EQUAL(s1,"Jokes");
  
  std::string s2(1,tcl.p_char_ntf);
  
  BOOST_CHECK_EQUAL(s2,"s");
  
  p_gl_char = s1[2];
  
  BOOST_CHECK_EQUAL(p_gl_char,'k');
  
  s2 += p_auto_value_class::p_st_char;
  
  BOOST_CHECK_EQUAL(s2,"so");
  
  tcl.p_char_ntf.fun = &TestCharacterNotification;
  tcl.p_char_ntf = 'i';
  
  BOOST_CHECK(TestCharacterNotificationWorked);
  }
  
void TestInt(p_auto_value_class & tcl) 
  {
  
  /*
  
  Integers can be tested with the full range of integer operations.
  
  */
  
  prop_auto<int,notification_boost_function> p_loc_int(563);
  
  BOOST_CHECK_EQUAL(p_gl_int_const,34662);
  BOOST_CHECK_EQUAL(p_auto_value_class::p_st_int_const,34);
  tcl.p_int = 44;
  tcl.p_int += 12;
  BOOST_CHECK_EQUAL(tcl.p_int,56);
  tcl.p_int -= 16;
  BOOST_CHECK_EQUAL(tcl.p_int,40);
  ++tcl.p_int;
  BOOST_CHECK_EQUAL(tcl.p_int,41);
  ++tcl.p_int;
  ++tcl.p_int;
  --tcl.p_int;
  BOOST_CHECK_EQUAL(tcl.p_int,42);
  tcl.p_int /= 3;
  BOOST_CHECK_EQUAL(tcl.p_int,14);
  tcl.p_int *= 5;
  BOOST_CHECK_EQUAL(tcl.p_int,70);
  
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
  p_loc_int.fun = &TestIntegerNotification;
  p_loc_int = 23611;
  BOOST_CHECK(TestIntegerNotificationWorked);
  
  tcl.p_int ^= 57;
  
  }
  
void TestDouble(p_auto_value_class & tcl) 
  {
  
  /*
  
  We can do the full range of arithmetic functionality with a double value
  
  */
  
  prop_auto<double> p_loc_double(45.37);
  ++p_loc_double;
  BOOST_CHECK_CLOSE(p_loc_double.get(),46.37,.1);
  BOOST_CHECK_CLOSE(tcl.p_double_ntf.get(),15.0,.1);
  p_gl_double = 2352.346;
  p_auto_value_class::p_st_double = 452.98;
  BOOST_CHECK_CLOSE(p_gl_double.get(),2352.346,.01);
  BOOST_CHECK_CLOSE(p_auto_value_class::p_st_double.get(),452.98,.1);
  --p_auto_value_class::p_st_double;
  BOOST_CHECK_CLOSE(p_auto_value_class::p_st_double.get(),451.98,.1);
  double d1(p_gl_double--);
  BOOST_CHECK_CLOSE(d1,2352.346,.01);
  BOOST_CHECK_CLOSE(p_gl_double.get(),2351.346,.01);
  d1 = tcl.p_double_ntf++;
  BOOST_CHECK_CLOSE(d1,15.0,.1);
  BOOST_CHECK_CLOSE(tcl.p_double_ntf.get(),16.0,.1);
  p_gl_double /= 36.7;
  BOOST_CHECK_CLOSE(p_gl_double.get(),64.069373,.1);
  p_loc_double *= 756.839; // 35094.624
  BOOST_CHECK_CLOSE(p_loc_double.get(),35094.624,.001);
  p_auto_value_class::p_st_double -= 2497.481;
  BOOST_CHECK_CLOSE(p_auto_value_class::p_st_double.get(),-2045.501,.01);
  tcl.p_double_ntf.fun = &TestDoubleNotification;
  tcl.p_double_ntf += 566639.298;
  BOOST_CHECK(TestDoubleNotificationWorked);
  
  }
  
void TestBool(p_auto_value_class & tcl) 
  {
  
  /*
  
  Bools are integral values, 
  either 0 (false) or 1 (true) in integer arithmetic,
  but are only tested here for true or false
  
  */
  
  prop_auto<bool> p_loc_bool(true);
  
  bool b1(tcl.p_bool);
  
  BOOST_CHECK(b1);
  
  bool b2(p_loc_bool);
  
  BOOST_CHECK(b2);
  
  b1 = p_gl_bool;
  
  BOOST_CHECK_EQUAL(b1,false);
  
  p_auto_value_class::p_st_bool_ntf.fun = &TestBoolNotification;
  p_auto_value_class::p_st_bool_ntf = b2;
  
  BOOST_CHECK(TestBoolNotificationWorked);
  
  bool b3(p_auto_value_class::p_st_bool_ntf);
  
  BOOST_CHECK(b3);
  
  }
  
void TestPointer(p_auto_value_class & tcl) 
  {
  
  /*
  
  Pointers can be assigned, incremented, decremented,
  and take part in simple arithmetic.
  
  */
  
  prop_auto<int *,notification_boost_function> p_loc_pointer(&p_gl_iarray[7]);
  prop_auto<int *> p_loc_pointer2(&p_gl_iarray[4]);
  
  BOOST_CHECK_EQUAL(*p_loc_pointer,7783);
  
  p_loc_pointer = &p_gl_iarray[6];
  
  BOOST_CHECK_EQUAL(*p_loc_pointer,230);
  BOOST_CHECK_EQUAL(p_loc_pointer - p_loc_pointer2,2);
  
  p_loc_pointer += 4;
  BOOST_CHECK_EQUAL(*p_loc_pointer,12);
  p_loc_pointer.fun = &TestPointerNotification;
  p_loc_pointer -= 2;
  BOOST_CHECK_EQUAL(*p_loc_pointer,22675);
  BOOST_CHECK(TestPointerNotificationWorked);
  
  gld_int = 489;
  BOOST_CHECK_EQUAL(*p_gl_pointer_const,489);
  BOOST_CHECK_EQUAL(*p_auto_value_class::p_st_pointer,53);
  ++p_auto_value_class::p_st_pointer;
  BOOST_CHECK_EQUAL(*p_auto_value_class::p_st_pointer,3456);
  BOOST_CHECK_EQUAL(*tcl.p_pointer,34);
  tcl.p_pointer -= 2;
  BOOST_CHECK_EQUAL(*tcl.p_pointer,13562);
  --tcl.p_pointer;
  BOOST_CHECK_EQUAL(*tcl.p_pointer,56);
  tcl.p_pointer += 11;
  BOOST_CHECK_EQUAL(*(tcl.p_pointer--),3452);
  BOOST_CHECK_EQUAL(*tcl.p_pointer,789);
  p_auto_value_class::p_st_pointer -= 4;
  BOOST_CHECK_EQUAL(*(p_auto_value_class::p_st_pointer++),5347);
  BOOST_CHECK_EQUAL(*p_auto_value_class::p_st_pointer,369);
  
  }
  
void TestEnum(p_auto_value_class & tcl) 
  {
  
  /*
  
  Enums are individual values. They can participate in 
  arithmetic expressions but here I will just test basic 
  usage.
  
  */
  
  prop_auto<test_enum> p_loc_enum(e_test_second);
  
  BOOST_CHECK_EQUAL(tcl.p_enum_const,e_test_second);
  BOOST_CHECK_EQUAL(p_loc_enum,e_test_second);
  p_loc_enum = e_test_third;
  BOOST_CHECK_EQUAL(p_loc_enum,e_test_third);
  BOOST_CHECK_EQUAL(p_auto_value_class::p_st_enum_ntf,e_test_third);
  p_auto_value_class::p_st_enum_ntf = e_test_first;
  BOOST_CHECK_EQUAL(p_auto_value_class::p_st_enum_ntf,e_test_first);
  test_enum te1(p_loc_enum);
  BOOST_CHECK_EQUAL(te1,e_test_third);
  
  te1 = p_auto_value_class::p_st_enum_ntf;
  BOOST_CHECK_EQUAL(te1,e_test_first);
  p_auto_value_class::p_st_enum_ntf.fun = &TestEnumNotification;
  p_auto_value_class::p_st_enum_ntf = e_test_third;
  BOOST_CHECK(TestEnumNotificationWorked);
  
  }
  
void TestPod(p_auto_value_class & tcl) 
  {
  
  /*
  
  Our POD class will be tested by changing fields 
  and passing to and from the properties.
  
  */
  
  prop_auto<test_pod> p_loc_pod(test_pod('^',981,4.3,false));
  
  BOOST_CHECK_EQUAL(p_loc_pod.get().an_int,981);
  BOOST_CHECK_EQUAL(p_loc_pod.get().a_char,'^');
  
  test_pod p1(p_auto_value_class::p_st_pod_const);
  
  BOOST_CHECK_EQUAL(p1.a_char,'y');
  BOOST_CHECK(p1.a_bool);
  BOOST_CHECK_EQUAL(p_auto_value_class::p_st_pod_const.get().an_int,45);
  BOOST_CHECK_CLOSE(p_auto_value_class::p_st_pod_const.get().a_double,32.6,.1);
  
  p1 = p_loc_pod;
  p1.an_int = 991;
  p_loc_pod = p1;
  BOOST_CHECK_EQUAL(p_loc_pod.get().an_int,991);
  
  BOOST_CHECK_EQUAL(tcl.p_pod.get().an_int,997);
  BOOST_CHECK_EQUAL(tcl.p_pod.get().a_char,'#');
  
  tcl.p_pod = test_pod();
  
  BOOST_CHECK_EQUAL(tcl.p_pod.get().a_double,0);
  
  p_gl_pod_ntf.fun = &TestPodNotification;
  p_gl_pod_ntf = test_pod('$',547,33.45,true);
  BOOST_CHECK(TestPodNotificationWorked);
  
  }
  
void test_auto_value_function()
  {
  
  p_auto_value_class tcl;
  
  TestCharacter(tcl);
  TestInt(tcl);
  TestDouble(tcl);
  TestBool(tcl);
  TestPointer(tcl);
  TestEnum(tcl);
  TestPod(tcl);
  
  }
  
boost::unit_test::test_suite* init_unit_test_suite( int, char* [] )
{
    boost::unit_test::test_suite* test = BOOST_TEST_SUITE( "Property Test Auto Value Suite" );

    test->add( BOOST_TEST_CASE( &test_auto_value_function ) );

    return test;
}

#include "test_global_data.cpp"
