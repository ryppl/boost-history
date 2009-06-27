// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign_zero, int_type, IntTypes)
{
  int_type x;
  x = "0";
  BOOST_CHECK_EQUAL(!x, true);
}
/*
BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign_dec1, int_type, IntTypes)
{
  int_type x;
  x = "269513460000009900000000";
  BOOST_CHECK_EQUAL(x, "269513460000009900000000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign_dec2, int_type, IntTypes)
{
  int_type x;
  x = "-269513460000009900000000";
  BOOST_CHECK_EQUAL(x, "-269513460000009900000000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign_oct1, int_type, IntTypes)
{
  int_type x;
  x = "012345676543210000001";
  BOOST_CHECK_EQUAL(x, "012345676543210000001");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign_oct2, int_type, IntTypes)
{
  int_type x;
  x = "-012345676543210000001";
  BOOST_CHECK_EQUAL(x, "-012345676543210000001");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign_hex1, int_type, IntTypes)
{
  int_type x;
  x = "0xabcdef123456789000000000005000000000000007ffffff";
  BOOST_CHECK_EQUAL(x, "0xabcdef123456789000000000005000000000000007ffffff");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign_hex2, int_type, IntTypes)
{
  int_type x;
  x = "-0xabcdef123456789000000000005000000000000007ffffff";
  BOOST_CHECK_EQUAL(x, "-0xabcdef123456789000000000005000000000000007ffffff");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign_hex3, int_type, IntTypes)
{
  int_type x;
  x = "-0xABCDEF00000123456798EEEEEEEEEE";
  BOOST_CHECK_EQUAL(x, "-0xABCDEF00000123456798EEEEEEEEEE");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign_hex4, int_type, IntTypes)
{
  int_type x;
  x = "-0Xabcdef00000123456798eeeeeeeeee";
  BOOST_CHECK_EQUAL(x, "-0Xabcdef00000123456798eeeeeeeeee");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign1, int_type, IntTypes)
{
  int_type x("-0x1");
  x = "0123456765432100000000000000000000001";
  BOOST_CHECK_EQUAL(x, "0123456765432100000000000000000000001");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign2, int_type, IntTypes)
{
  int_type x("-0xabcedf0303030300000000000000");
  x = "01";
  BOOST_CHECK_EQUAL(x, "01");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign3, int_type, IntTypes)
{
  int_type x("0xabcedf0303030300000000000000");
  x = "-500";
  BOOST_CHECK_EQUAL(x, "-500");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_assign4, int_type, IntTypes)
{
  int_type x("0x0");
  x = "-0x500aaaaaaaaaaaa000000";
  BOOST_CHECK_EQUAL(x, "-0x500aaaaaaaaaaaa000000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(assign1, int_type, IntTypes)
{
  int_type x;
  x.assign("123456789876543210000001", std::ios::dec);
  BOOST_CHECK_EQUAL(x, "123456789876543210000001");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(assign2, int_type, IntTypes)
{
  int_type x;
  x.assign("abcdefabcdef1234567890", std::ios::hex);
  BOOST_CHECK_EQUAL(x, "0xabcdefabcdef1234567890");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(assign3, int_type, IntTypes)
{
  int_type x("-564897123123456456789789789897");
  x.assign("1234567000000000000000000000000077", std::ios::oct);
  BOOST_CHECK_EQUAL(x, "01234567000000000000000000000000077");
}
*/
