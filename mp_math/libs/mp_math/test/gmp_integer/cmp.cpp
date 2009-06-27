// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_eq1, int_type, IntTypes)
{
  const int_type x("0");
  const int_type y("0");
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_eq2, int_type, IntTypes)
{
  const int_type x("1");
  const int_type y("1");
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_eq3, int_type, IntTypes)
{
  const int_type x("0xeeffeeeeee000000000000001");
  const int_type y("0xeeffeeeeee000000000000001");
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_eq4, int_type, IntTypes)
{
  const int_type x("-0xeeffeeeeee000000000000001");
  const int_type y("-0xeeffeeeeee000000000000001");
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ne1, int_type, IntTypes)
{
  const int_type x("0");
  const int_type y("1");
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ne2, int_type, IntTypes)
{
  const int_type x("0xaaaaaaaaaaaaaaeeeeeeeeeeeeeee");
  const int_type y("0xaaaaaaaaaaaaaabbbbbbbbbbbbbb8");
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ne3, int_type, IntTypes)
{
  const int_type x("1");
  const int_type y("-1");
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_lt1, int_type, IntTypes)
{
  const int_type x("12");
  const int_type y("13");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_lt2, int_type, IntTypes)
{
  const int_type x("1");
  const int_type y("123456789");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_lt3, int_type, IntTypes)
{
  const int_type x("0x100000000000000000000");
  const int_type y("0x100000000000000000001");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_lt4, int_type, IntTypes)
{
  const int_type x("15");
  const int_type y("1023");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_lt5, int_type, IntTypes)
{
  const int_type x("-1");
  const int_type y("0");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_lt6, int_type, IntTypes)
{
  const int_type x("-0x589789");
  const int_type y("0x589789");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_gt1, int_type, IntTypes)
{
  const int_type x("1");
  const int_type y("0");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_gt2, int_type, IntTypes)
{
  const int_type x("0xfffffffffffffffffffffffff");
  const int_type y("0");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_gt3, int_type, IntTypes)
{
  const int_type x("0xffffffffffffffeeeeeeeeeee1");
  const int_type y("0xffffffffffffffeeeeeeeeeee0");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_gt4, int_type, IntTypes)
{
  const int_type x("1");
  const int_type y("-1");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_gt5, int_type, IntTypes)
{
  const int_type x("0xffffffffffffffeeeeeeeeeee1");
  const int_type y("-0xffffffffffffffeeeeeeeeeee0");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_le1, int_type, IntTypes)
{
  const int_type x("0");
  const int_type y("0");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_le2, int_type, IntTypes)
{
  const int_type x("0");
  const int_type y("1");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_le3, int_type, IntTypes)
{
  const int_type x("0x123456789fffffffffff");
  const int_type y("0x123456789fffffffffff");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_le4, int_type, IntTypes)
{
  const int_type x("0x11");
  const int_type y("0x49941faaaaaaaa332134");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_le5, int_type, IntTypes)
{
  const int_type x("-0x123456789fffffffffff");
  const int_type y("0x123456789fffffffffff");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_le6, int_type, IntTypes)
{
  const int_type x("-0x123456789fffffffffff");
  const int_type y("-0x123456789fffffffffff");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ge1, int_type, IntTypes)
{
  const int_type x("1");
  const int_type y("1");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ge2, int_type, IntTypes)
{
  const int_type x("0xa0a0");
  const int_type y("0");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ge3, int_type, IntTypes)
{
  const int_type x("0xff00ff00000000000000001ff");
  const int_type y("0xff00ff00000000000000000ff");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ge4, int_type, IntTypes)
{
  const int_type x("0xff00ff00000000000000000ff");
  const int_type y("-0xff00ff00000000000000000ff");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ge5, int_type, IntTypes)
{
  const int_type x("-0xff00ff00000000000000000ff");
  const int_type y("-0xff00ff00000000000000000ff");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_integral1, int_type, IntTypes)
{
  const int_type x("0");
  const int y = 0;
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_integral2, int_type, IntTypes)
{
  const int_type x("20");
  const unsigned int y = 20;
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_integral3, int_type, IntTypes)
{
  const int_type x("30000");
  const int y = 30000;
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_integral4, int_type, IntTypes)
{
  const int_type x("0xffff0000");
  const unsigned long y = 0xffff0000;
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_integral5, int_type, IntTypes)
{
  const int_type x("-30000");
  const int y = -30000;
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ne_integral1, int_type, IntTypes)
{
  const int_type x("1");
  const int y = 0;
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ne_integral2, int_type, IntTypes)
{
  const int_type x("0");
  const int y = -10000;
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ne_integral3, int_type, IntTypes)
{
  const int_type x("-10000");
  const unsigned int y = 10000;
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ne_integral4, int_type, IntTypes)
{
  const int_type x("-10000");
  const int y = -10001;
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_lt_integral1, int_type, IntTypes)
{
  const int_type x("0");
  const int y = 1;
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_lt_integral2, int_type, IntTypes)
{
  const int_type x("123456789");
  const unsigned long y = 123456790;
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_lt_integral3, int_type, IntTypes)
{
  const int_type x("-0x500");
  const int y = -0x400;
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_lt_integral4, int_type, IntTypes)
{
  const int_type x("-0x500");
  const unsigned int y = 0x400;
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_gt_integral1, int_type, IntTypes)
{
  const int_type x("1");
  const int y = -1;
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_gt_integral2, int_type, IntTypes)
{
  const int_type x("0x5000000addddddd");
  const unsigned int y = 1156;
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_gt_integral3, int_type, IntTypes)
{
  const int_type x("-1321");
  const int y = -1500;
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_le_integral1, int_type, IntTypes)
{
  const int_type x("0");
  const int y = 0;
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_le_integral2, int_type, IntTypes)
{
  const int_type x("0xffff");
  const unsigned int y = 0xffff;
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_le_integral3, int_type, IntTypes)
{
  const int_type x("0xaa0");
  const unsigned int y = 0xf000;
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_le_integral4, int_type, IntTypes)
{
  const int_type x("-0x100");
  const int y = -0x100;
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_le_integral5, int_type, IntTypes)
{
  const int_type x("-0x100");
  const int y = 0x100;
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_le_integral6, int_type, IntTypes)
{
  const int_type x("-0x100");
  const unsigned int y = 0x100;
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ge_integral1, int_type, IntTypes)
{
  const int_type x("0");
  const int y = 0;
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ge_integral2, int_type, IntTypes)
{
  const int_type x("100");
  const int y = -100;
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ge_integral3, int_type, IntTypes)
{
  const int_type x("0x56a4f564aaa445456");
  const unsigned int y = 0xaab;
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ge_integral4, int_type, IntTypes)
{
  const int_type x("-100");
  const int y = -100;
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_eq_mp_int1, int_type, IntTypes)
{
  const int x = 0;
  const int_type y("0");
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_eq_mp_int2, int_type, IntTypes)
{
  const unsigned int x = 1000;
  const int_type y("1000");
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_eq_mp_int3, int_type, IntTypes)
{
  const int x = -1000;
  const int_type y("-1000");
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ne_mp_int1, int_type, IntTypes)
{
  const int x = 0;
  const int_type y("0x1000");
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ne_mp_int2, int_type, IntTypes)
{
  const int x = -500;
  const int_type y("500");
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ne_mp_int3, int_type, IntTypes)
{
  const unsigned int x = 500;
  const int_type y("0xcaaff500");
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ne_mp_int4, int_type, IntTypes)
{
  const int x = 500;
  const int_type y("-500");
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_lt_mp_int1, int_type, IntTypes)
{
  const unsigned int x = 500;
  const int_type y("0xcaaff500");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_lt_mp_int2, int_type, IntTypes)
{
  const int x = -500;
  const int_type y("0");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_lt_mp_int3, int_type, IntTypes)
{
  const int x = -0x800;
  const int_type y("-0x500");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_gt_mp_int1, int_type, IntTypes)
{
  const int x = 1;
  const int_type y("0");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_gt_mp_int2, int_type, IntTypes)
{
  const unsigned int x = 1000;
  const int_type y("999");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_gt_mp_int3, int_type, IntTypes)
{
  const int x = -0x999;
  const int_type y("-0x1000");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_le_mp_int1, int_type, IntTypes)
{
  const unsigned int x = 999;
  const int_type y("999");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_le_mp_int2, int_type, IntTypes)
{
  const int x = -1;
  const int_type y("0x99999999999999");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_le_mp_int3, int_type, IntTypes)
{
  const int x = -0x999;
  const int_type y("-0x999");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_le_mp_int4, int_type, IntTypes)
{
  const int x = -0x1000;
  const int_type y("-0x999");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ge_mp_int1, int_type, IntTypes)
{
  const unsigned int x = 999;
  const int_type y("999");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ge_mp_int2, int_type, IntTypes)
{
  const int x = 1023;
  const int_type y("1010");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ge_mp_int3, int_type, IntTypes)
{
  const int x = -0x1023;
  const int_type y("-0x1023");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ge_mp_int4, int_type, IntTypes)
{
  const int x = 0x1023;
  const int_type y("-0x1023");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ge_mp_int5, int_type, IntTypes)
{
  const unsigned int x = 0x1023;
  const int_type y("-0x102305615645ff");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_char_ptr1, int_type, IntTypes)
{
  const int_type x("0");
  const char* y = "0";
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_char_ptr2, int_type, IntTypes)
{
  const int_type x("0x456561fcaa547650456456");
  const char* y = "0x456561fcaa547650456456";
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_char_ptr3, int_type, IntTypes)
{
  const int_type x("-0x500000");
  const char* y = "-0x500000";
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ne_char_ptr1, int_type, IntTypes)
{
  const int_type x("15");
  const char* y = "22";
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ne_char_ptr2, int_type, IntTypes)
{
  const int_type x("0x456561fcaa547650456456");
  const char* y = "-0x456561fcaa547650456456";
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ne_char_ptr3, int_type, IntTypes)
{
  const int_type x("-0x456561fcaa547650456456");
  const char* y = "0x456561fcaa547650456456";
  BOOST_CHECK_NE(x, y);
}

