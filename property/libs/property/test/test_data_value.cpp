#include <boost/test/unit_test.hpp>
#include "test_data_value.hpp"
#include "test_global_data.hpp"

int p_data_value_class::p_st_iarray[9] = {757,9,5347,369,1174,53,3456,443,6};
int p_gl_iarray[] = {7,98,347,289,14,5763,230,7783,22675,59,12};

prop_data<char const,&p_data_class::p_char_2> p_gl_char_const;
prop_data<int,&gld_int> p_gl_int(34662);
prop_data<double const,&gld_double> p_gl_double_const;
prop_data<bool,&p_data_class::p_bool,default_policy_tag,read_tag> p_gl_bool;
prop_data<int *,&gld_pointer,notification_boost_function> p_gl_pointer_ntf(&gld_int);
prop_data<test_enum,&p_data_class::p_enum_2,notification_boost_function> p_gl_enum_ntf(e_test_third);
prop_data<test_pod,&gld_pod,notification_boost_function> p_gl_pod_ntf;

p_data_value_class::p_data_value_class() :
  p_double_ntf(15),
  p_bool(true),
  p_pointer(&p_iarray[3]),
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

prop_data<char,&p_data_class::p_char> p_data_value_class::p_st_char('T');
prop_data<int,&p_data_class::p_int> p_data_value_class::p_st_int(34);
prop_data<double const,&p_data_class::p_double_2> p_data_value_class::p_st_double_const;
prop_data<bool,&gld_bool> p_data_value_class::p_st_bool;
prop_data<int *,&p_data_class::p_pointer_2> p_data_value_class::p_st_pointer(&p_st_iarray[5]);
prop_data<test_enum,&gld_enum> p_data_value_class::p_st_enum(e_test_third);
prop_data<test_pod const,&p_data_class::p_pod_2> p_data_value_class::p_st_pod_const;

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

void TestCharacter(p_data_value_class & tcl) 
  {
  
  /*
  
  Although a character is considered an integral type, 
  doing integral arithmetic on it is really not normal.
  So we show functionality based on characters being part
  of C++ strings.
  
  */
  
  prop_data<char,&p_data_class::p_char_3,notification_boost_function> p_loc_char_ntf('J');
  std::string s1("Terry");
  
  s1[0] = p_loc_char_ntf;
  
  BOOST_CHECK_EQUAL(s1,"Jerry");
  
  p_data_class::p_char_2 = 'k';
  
  s1[3] = p_gl_char_const;
  
  BOOST_CHECK_EQUAL(s1,"Jerky");
  
  p_loc_char_ntf = s1[1];
  tcl.p_char = 's';
  p_data_value_class::p_st_char = 'o';
  
  s1[1] = p_data_value_class::p_st_char;
  s1[2] = p_gl_char_const;
  s1[3] = p_loc_char_ntf;
  s1[4] = tcl.p_char;
  
  BOOST_CHECK_EQUAL(s1,"Jokes");
  
  std::string s2(1,tcl.p_char);
  
  BOOST_CHECK_EQUAL(s2,"s");
  
  s2 += p_data_value_class::p_st_char;
  
  BOOST_CHECK_EQUAL(s2,"so");
  
  p_loc_char_ntf.fun = &TestCharacterNotification;
  p_loc_char_ntf = 'i';
  
  BOOST_CHECK(TestCharacterNotificationWorked);
  
  }
  
void TestInt(p_data_value_class & tcl) 
  {
  
  /*
  
  Integers can be tested with the full range of integer operations.
  
  */
  
  prop_data<int,&p_data_class::p_int_3,notification_boost_function> p_loc_int(563);
  
  BOOST_CHECK_EQUAL(p_gl_int,34662);
  BOOST_CHECK_EQUAL(p_data_value_class::p_st_int,34);
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
  
void TestDouble(p_data_value_class & tcl) 
  {
  
  /*
  
  We can do the full range of arithmetic functionality with a double value
  
  */
  
  prop_data<double,&p_data_class::p_double_3> p_loc_double(45.37);
  ++p_loc_double;
  BOOST_CHECK_CLOSE(p_loc_double.get(),46.37,.1);
  BOOST_CHECK_CLOSE(tcl.p_double_ntf.get(),15.0,.1);
  gld_double = 2352.346;
  p_data_class::p_double_2 = 452.98;
  BOOST_CHECK_CLOSE(p_gl_double_const.get(),2352.346,.01);
  BOOST_CHECK_CLOSE(p_data_value_class::p_st_double_const.get(),452.98,.1);
  double d1(p_gl_double_const);
  BOOST_CHECK_CLOSE(d1,2352.346,.01);
  d1 = tcl.p_double_ntf++;
  BOOST_CHECK_CLOSE(d1,15.0,.1);
  BOOST_CHECK_CLOSE(tcl.p_double_ntf.get(),16.0,.1);
  p_loc_double = p_gl_double_const;
  p_loc_double /= 36.7;
  BOOST_CHECK_CLOSE(p_loc_double.get(),64.069373,.1);
  p_loc_double = 46.37;
  p_loc_double *= 756.839;
  BOOST_CHECK_CLOSE(p_loc_double.get(),35094.624,.001);
  tcl.p_double_ntf.fun = &TestDoubleNotification;
  tcl.p_double_ntf += 566639.298;
  BOOST_CHECK(TestDoubleNotificationWorked);
  
  }
  
void TestBool(p_data_value_class & tcl) 
  {
  
  /*
  
  Bools are integral values, 
  either 0 (false) or 1 (true) in integer arithmetic,
  but are only tested here for true or false
  
  */
  
  prop_data<bool,&p_data_class::p_bool_3,notification_boost_function> p_loc_bool(true);
  
  bool b1(tcl.p_bool);
  
  BOOST_CHECK(b1);
  
  bool b2(p_loc_bool);
  
  BOOST_CHECK(b2);
  
  p_data_class::p_bool = false;
  
  b1 = p_gl_bool;
  
  BOOST_CHECK_EQUAL(b1,false);
  
  p_data_value_class::p_st_bool = b2;
  
  BOOST_CHECK(p_data_value_class::p_st_bool);
  
  p_loc_bool.fun = &TestBoolNotification;
  p_loc_bool = b2;
  
  BOOST_CHECK(TestBoolNotificationWorked);
  
  bool b3(p_loc_bool);
  
  BOOST_CHECK(b3);
  
  }
  
void TestPointer(p_data_value_class & tcl) 
  {
  
  /*
  
  Pointers can be assigned, incremented, decremented,
  and take part in simple arithmetic.
  
  */
  
  prop_data<int *,&p_data_class::p_pointer_3,notification_boost_function> p_loc_pointer(&p_gl_iarray[7]);
  prop_data<int *,&p_data_class::p_pointer_4> p_loc_pointer2(&p_gl_iarray[4]);
  
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
  BOOST_CHECK_EQUAL(*p_gl_pointer_ntf,489);
  BOOST_CHECK_EQUAL(*p_data_value_class::p_st_pointer,53);
  ++p_data_value_class::p_st_pointer;
  BOOST_CHECK_EQUAL(*p_data_value_class::p_st_pointer,3456);
  BOOST_CHECK_EQUAL(*tcl.p_pointer,34);
  tcl.p_pointer -= 2;
  BOOST_CHECK_EQUAL(*tcl.p_pointer,13562);
  --tcl.p_pointer;
  BOOST_CHECK_EQUAL(*tcl.p_pointer,56);
  tcl.p_pointer += 11;
  BOOST_CHECK_EQUAL(*(tcl.p_pointer--),3452);
  BOOST_CHECK_EQUAL(*tcl.p_pointer,789);
  p_data_value_class::p_st_pointer -= 4;
  BOOST_CHECK_EQUAL(*(p_data_value_class::p_st_pointer++),5347);
  BOOST_CHECK_EQUAL(*p_data_value_class::p_st_pointer,369);
  
  }
  
void TestEnum(p_data_value_class & tcl)
  {
  
  /*
  
  Enums are individual values. They can participate in 
  arithmetic expressions but here I will just test basic 
  usage.
  
  */
  
  prop_data<test_enum,&p_data_class::p_enum_3> p_loc_enum(e_test_second);
  
  p_data_class::p_enum = e_test_second;
  
  BOOST_CHECK_EQUAL(tcl.p_enum_const,e_test_second);
  BOOST_CHECK_EQUAL(p_loc_enum,e_test_second);
  p_loc_enum = e_test_third;
  BOOST_CHECK_EQUAL(p_loc_enum,e_test_third);
  BOOST_CHECK_EQUAL(p_data_value_class::p_st_enum,e_test_third);
  p_data_value_class::p_st_enum = e_test_first;
  BOOST_CHECK_EQUAL(p_data_value_class::p_st_enum,e_test_first);
  test_enum te1(p_loc_enum);
  BOOST_CHECK_EQUAL(te1,e_test_third);
  
  te1 = p_data_value_class::p_st_enum;
  BOOST_CHECK_EQUAL(te1,e_test_first);
  p_gl_enum_ntf.fun = &TestEnumNotification;
  p_gl_enum_ntf = e_test_third;
  BOOST_CHECK(TestEnumNotificationWorked);
  
  }
  
void TestPod(p_data_value_class & tcl) 
  {
  
  /*
  
  Our POD class will be tested by changing fields 
  and passing to and from the properties.
  
  */
  
  prop_data<test_pod,&p_data_class::p_pod_3> p_loc_pod(test_pod('^',981,4.3,false));
  
  BOOST_CHECK_EQUAL(p_loc_pod.get().an_int,981);
  BOOST_CHECK_EQUAL(p_loc_pod.get().a_char,'^');
  
  p_data_class::p_pod_2 = test_pod('y',45,32.6,true);
  
  test_pod p1(p_data_value_class::p_st_pod_const);
  
  BOOST_CHECK_EQUAL(p1.a_char,'y');
  BOOST_CHECK(p1.a_bool);
  BOOST_CHECK_EQUAL(p_data_value_class::p_st_pod_const.get().an_int,45);
  BOOST_CHECK_CLOSE(p_data_value_class::p_st_pod_const.get().a_double,32.6,.1);
  
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

boost::unit_test::test_suite* init_unit_test_suite( int , char* [] )
{
    boost::unit_test::test_suite* test = BOOST_TEST_SUITE( "Property Test Data Value Function Suite" );

    test->add( BOOST_TEST_CASE( &test_data_value_function ) );

    return test;
}

#include "test_global_data.cpp"
#include "test_data_impl.cpp"
