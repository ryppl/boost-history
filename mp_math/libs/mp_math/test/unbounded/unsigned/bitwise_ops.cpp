// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(and_op1, uint_type, UIntTypes)
{
  const uint_type x("0x00ff0000000f");
  const uint_type y("0xffffffffffff");
  const uint_type z = x & y;
  BOOST_CHECK_EQUAL(z, x);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(and_op2, uint_type, UIntTypes)
{
  const uint_type x("0x00ff0000000ffffffffff");
  const uint_type y(         "0xffffffffffff");
  const uint_type z = x & y;
  BOOST_CHECK_EQUAL(z, "0xffffffffff");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(or_op1, uint_type, UIntTypes)
{
  const uint_type x("0x00ff0000000f");
  const uint_type y("0xffffffffffff");
  const uint_type z = x | y;
  BOOST_CHECK_EQUAL(z, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(or_op2, uint_type, UIntTypes)
{
  const uint_type x("0x00ff0000000ffffffffff");
  const uint_type y(         "0xaaffffffffff");
  const uint_type z = x | y;
  BOOST_CHECK_EQUAL(z, "0x00ff00000aaffffffffff");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(xor_op1, uint_type, UIntTypes)
{
  const uint_type x("0x00ff0000000f");
  const uint_type y("0xffffffffffff");
  const uint_type z = x ^ y;
  BOOST_CHECK_EQUAL(z, "0xff00fffffff0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(xor_op2, uint_type, UIntTypes)
{
  const uint_type x("0x00ff0000000ffffffffff");
  const uint_type y(         "0x33aaffffffff");
  const uint_type z = x ^ y;
  BOOST_CHECK_EQUAL(z,"0x00ff00000335500000000");
}

