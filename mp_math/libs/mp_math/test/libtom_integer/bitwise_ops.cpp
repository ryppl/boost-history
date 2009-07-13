// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(and_op1, int_type, IntTypes)
{
  const int_type x("0x00ff0000000f");
  const int_type y("-0xffffffffffff");
  const int_type z = x & y;
  BOOST_CHECK_EQUAL(z, x);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(and_op2, int_type, IntTypes)
{
  const int_type x("0x00ff0000000ffffffffff");
  const int_type y(         "0xffffffffffff");
  const int_type z = x & y;
  BOOST_CHECK_EQUAL(z, "0xffffffffff");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(or_op1, int_type, IntTypes)
{
  const int_type x("0x00ff0000000f");
  const int_type y("-0xffffffffffff");
  const int_type z = x | y;
  BOOST_CHECK_EQUAL(z, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(or_op2, int_type, IntTypes)
{
  const int_type x("0x00ff0000000ffffffffff");
  const int_type y(         "0xaaffffffffff");
  const int_type z = x | y;
  BOOST_CHECK_EQUAL(z, "0x00ff00000aaffffffffff");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(xor_op1, int_type, IntTypes)
{
  const int_type x("0x00ff0000000f");
  const int_type y("0xffffffffffff");
  const int_type z = x ^ y;
  BOOST_CHECK_EQUAL(z, "0xff00fffffff0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(xor_op2, int_type, IntTypes)
{
  const int_type x("0x00ff0000000ffffffffff");
  const int_type y(         "0x33aaffffffff");
  const int_type z = x ^ y;
  BOOST_CHECK_EQUAL(z,"0x00ff00000335500000000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(xor_op3, int_type, IntTypes)
{
  const int_type x("-0x00ff0000000f");
  const int_type y("-0xffffffffffff");
  const int_type z = x ^ y;
  BOOST_CHECK_EQUAL(z, "0xff00fffffff0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(xor_op4, int_type, IntTypes)
{
  const int_type x("0x00ff0000000f");
  const int_type y("-0xffffffffffff");
  const int_type z = x ^ y;
  BOOST_CHECK_EQUAL(z, "-0xff00fffffff0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(compl1, int_type, IntTypes)
{
  int_type x("-0x6f0000000000000fabab");
  x = ~x;
  BOOST_CHECK_EQUAL(x, "0x10fffffffffffff05454");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(compl2, int_type, IntTypes)
{
  int_type x("0");
  x = ~x;
  BOOST_CHECK_EQUAL(x, "-1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(compl3, int_type, IntTypes)
{
  int_type x("1");
  x = ~x;
  BOOST_CHECK_EQUAL(x, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(compl4, int_type, IntTypes)
{
  int_type x("18446744073709551616"); // 2^65
  x = ~x;
  BOOST_CHECK_EQUAL(x, "-0xFFFFFFFFFFFFFFFF");
}

