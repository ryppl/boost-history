// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_eq1, uint_type, UIntTypes)
{
  const uint_type x("0");
  const uint_type y("0");
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_eq2, uint_type, UIntTypes)
{
  const uint_type x("1");
  const uint_type y("1");
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_eq3, uint_type, UIntTypes)
{
  const uint_type x("0xeeffeeeeee000000000000001");
  const uint_type y("0xeeffeeeeee000000000000001");
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ne1, uint_type, UIntTypes)
{
  const uint_type x("0");
  const uint_type y("1");
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ne2, uint_type, UIntTypes)
{
  const uint_type x("0xaaaaaaaaaaaaaaeeeeeeeeeeeeeee");
  const uint_type y("0xaaaaaaaaaaaaaabbbbbbbbbbbbbb8");
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_lt1, uint_type, UIntTypes)
{
  const uint_type x("12");
  const uint_type y("13");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_lt2, uint_type, UIntTypes)
{
  const uint_type x("1");
  const uint_type y("123456789");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_lt3, uint_type, UIntTypes)
{
  const uint_type x("0x100000000000000000000");
  const uint_type y("0x100000000000000000001");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_lt4, uint_type, UIntTypes)
{
  const uint_type x("15");
  const uint_type y("1023");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_gt1, uint_type, UIntTypes)
{
  const uint_type x("1");
  const uint_type y("0");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_gt2, uint_type, UIntTypes)
{
  const uint_type x("0xfffffffffffffffffffffffff");
  const uint_type y("0");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_gt3, uint_type, UIntTypes)
{
  const uint_type x("0xffffffffffffffeeeeeeeeeee1");
  const uint_type y("0xffffffffffffffeeeeeeeeeee0");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_le1, uint_type, UIntTypes)
{
  const uint_type x("0");
  const uint_type y("0");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_le2, uint_type, UIntTypes)
{
  const uint_type x("0");
  const uint_type y("1");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_le3, uint_type, UIntTypes)
{
  const uint_type x("0x123456789fffffffffff");
  const uint_type y("0x123456789fffffffffff");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_le4, uint_type, UIntTypes)
{
  const uint_type x("0x11");
  const uint_type y("0x49941faaaaaaaa332134");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ge1, uint_type, UIntTypes)
{
  const uint_type x("1");
  const uint_type y("1");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ge2, uint_type, UIntTypes)
{
  const uint_type x("0xa0a0");
  const uint_type y("0");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(cmp_ge3, uint_type, UIntTypes)
{
  const uint_type x("0xff00ff00000000000000001ff");
  const uint_type y("0xff00ff00000000000000000ff");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_integral1, uint_type, UIntTypes)
{
  const uint_type x("0");
  const int y = 0;
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_integral2, uint_type, UIntTypes)
{
  const uint_type x("20");
  const unsigned int y = 20;
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_integral3, uint_type, UIntTypes)
{
  const uint_type x("30000");
  const int y = 30000;
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_integral4, uint_type, UIntTypes)
{
  const uint_type x("0xffff0000");
  const unsigned long y = 0xffff0000;
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ne_integral1, uint_type, UIntTypes)
{
  const uint_type x("1");
  const int y = 0;
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ne_integral2, uint_type, UIntTypes)
{
  const uint_type x("0");
  const int y = -10000;
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_lt_integral1, uint_type, UIntTypes)
{
  const uint_type x("0");
  const int y = 1;
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_lt_integral2, uint_type, UIntTypes)
{
  const uint_type x("123456789");
  const unsigned long y = 123456790;
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_gt_integral1, uint_type, UIntTypes)
{
  const uint_type x("1");
  const int y = -1;
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_gt_integral2, uint_type, UIntTypes)
{
  const uint_type x("0x5000000addddddd");
  const unsigned int y = 1156;
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_le_integral1, uint_type, UIntTypes)
{
  const uint_type x("0");
  const int y = 0;
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_le_integral2, uint_type, UIntTypes)
{
  const uint_type x("0xffff");
  const unsigned int y = 0xffff;
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_le_integral3, uint_type, UIntTypes)
{
  const uint_type x("0xaa0");
  const unsigned int y = 0xf000;
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ge_integral1, uint_type, UIntTypes)
{
  const uint_type x("0");
  const int y = 0;
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ge_integral2, uint_type, UIntTypes)
{
  const uint_type x("100");
  const int y = -100;
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ge_integral3, uint_type, UIntTypes)
{
  const uint_type x("0x56a4f564aaa445456");
  const unsigned int y = 0xaab;
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_eq_mp_int1, uint_type, UIntTypes)
{
  const int x = 0;
  const uint_type y("0");
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_eq_mp_int2, uint_type, UIntTypes)
{
  const unsigned int x = 1000;
  const uint_type y("1000");
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ne_mp_int1, uint_type, UIntTypes)
{
  const int x = 0;
  const uint_type y("0x1000");
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ne_mp_int2, uint_type, UIntTypes)
{
  const int x = -500;
  const uint_type y("500");
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ne_mp_int3, uint_type, UIntTypes)
{
  const unsigned int x = 500;
  const uint_type y("0xcaaff500");
  BOOST_CHECK_NE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_lt_mp_int1, uint_type, UIntTypes)
{
  const unsigned int x = 500;
  const uint_type y("0xcaaff500");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_lt_mp_int2, uint_type, UIntTypes)
{
  const int x = -500;
  const uint_type y("0");
  BOOST_CHECK_LT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_gt_mp_int1, uint_type, UIntTypes)
{
  const int x = 1;
  const uint_type y("0");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_gt_mp_int2, uint_type, UIntTypes)
{
  const unsigned int x = 1000;
  const uint_type y("999");
  BOOST_CHECK_GT(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_le_mp_int1, uint_type, UIntTypes)
{
  const unsigned int x = 999;
  const uint_type y("999");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_le_mp_int2, uint_type, UIntTypes)
{
  const int x = -1;
  const uint_type y("0x99999999999999");
  BOOST_CHECK_LE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ge_mp_int1, uint_type, UIntTypes)
{
  const unsigned int x = 999;
  const uint_type y("999");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(integral_ge_mp_int2, uint_type, UIntTypes)
{
  const int x = 1023;
  const uint_type y("1010");
  BOOST_CHECK_GE(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_char_ptr1, uint_type, UIntTypes)
{
  const uint_type x("0");
  const char* y = "0";
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_eq_char_ptr2, uint_type, UIntTypes)
{
  const uint_type x("0x456561fcaa547650456456");
  const char* y = "0x456561fcaa547650456456";
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ne_char_ptr1, uint_type, UIntTypes)
{
  const uint_type x("15");
  const char* y = "22";
  BOOST_CHECK_NE(x, y);
}

/*BOOST_AUTO_TEST_CASE_TEMPLATE(mp_int_ne_char_ptr2, uint_type, UIntTypes)
{
  const uint_type x("0x456561fcaa547650456456");
  const char* y = "-0x456561fcaa547650456456";
  BOOST_CHECK_NE(x, y);
}*/

