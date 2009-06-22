// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(division_by_zero, uint_type, UIntTypes)
{
  const uint_type x("0x201abcff00aaffffff");
  const uint_type y("0");
  BOOST_CHECK_THROW(x / y, std::domain_error);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(divide1, uint_type, UIntTypes)
{
  const uint_type x("987777");
  const uint_type y("123456");
  const uint_type z = x / y;
  BOOST_CHECK_EQUAL(z, "8");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(divide2, uint_type, UIntTypes)
{
  const uint_type x("263825472");
  const uint_type y("123456");
  const uint_type z = x / y;
  BOOST_CHECK_EQUAL(z, "2137");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(divide3, uint_type, UIntTypes)
{
  const uint_type x("0x89ab89745cc653de58eecc8f8a874120065ea545f6f5f");
  const uint_type y("0x1889ba8a789456adfc8005b1");
  const uint_type z = x / y;
  BOOST_CHECK_EQUAL(z, "0x59c48aa7a1446110b31f70");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(divide4, uint_type, UIntTypes)
{
  const uint_type x("0x1889ba8a789456adfc8005b1");
  const uint_type y("0x1889ba8a789456adfc8005b2");
  const uint_type z = x / y;
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(divide5, uint_type, UIntTypes)
{
  const uint_type x("0x201abcff00aaffffff");
  const uint_type y("0x1fffffffffffffff");
  const uint_type z = x / y;
  BOOST_CHECK_EQUAL(z, "256");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(divide6, uint_type, UIntTypes)
{
  const uint_type x(
      "0xffffffffffffffff00000000000000000000000ffffffffffff00000000000000000000"
      "000000000000000eeeeeeeeeeeeee0");
  const uint_type y("0xffffffff00000000000");
  const uint_type z = x / y;
  BOOST_CHECK_EQUAL(z,
      "0x100000001000000000000000000000000000000100000000ffff0000ffff0000ffff00"
      "00ffff0000fff");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(shift_right1, uint_type, UIntTypes)
{
  uint_type x("263825472");
  x >>= 3;
  BOOST_CHECK_EQUAL(x, "32978184");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mod_zero, uint_type, UIntTypes)
{
  const uint_type x("987777");
  const uint_type y("0");
  BOOST_CHECK_THROW(x % y, std::domain_error);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mod1, uint_type, UIntTypes)
{
  const uint_type x("987777");
  const uint_type y("123456");
  const uint_type z = x % y;
  BOOST_CHECK_EQUAL(z, "129");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mod2, uint_type, UIntTypes)
{
  const uint_type x("0");
  const uint_type y("5");
  const uint_type z = x % y;
  BOOST_CHECK_EQUAL(z, "0");
}

