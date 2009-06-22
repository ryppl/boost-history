// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(construct_from_zero, uint_type, UIntTypes)
{
  const uint_type x(0);
  BOOST_CHECK_EQUAL(!x, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_signed_char_max, uint_type, UIntTypes)
{
  const signed char x = std::numeric_limits<signed char>::max();
  const uint_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_unsigned_char_max, uint_type, UIntTypes)
{
  const unsigned char x = std::numeric_limits<unsigned char>::max();
  const uint_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_int_max, uint_type, UIntTypes)
{
  const int x = std::numeric_limits<int>::max();
  const uint_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_unsigned_int_max, uint_type, UIntTypes)
{
  const unsigned int x = std::numeric_limits<unsigned int>::max();
  const uint_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_signed_integral1, uint_type, UIntTypes)
{
  const uint_type x("987777");
  const uint_type z = x + 1;
  BOOST_CHECK_EQUAL(z, "987778");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_signed_integral2, uint_type, UIntTypes)
{
  const uint_type x("987777");
  const uint_type z = x + (-1);
  BOOST_CHECK_EQUAL(z, "987776");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_unsigned_integral1, uint_type, UIntTypes)
{
  const uint_type x("9999999");
  const uint_type z = x + 1U;
  BOOST_CHECK_EQUAL(z, "10000000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_signed_char_max, uint_type, UIntTypes)
{
  const uint_type x("0");
  const uint_type z = x + std::numeric_limits<signed char>::max();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<signed char>::max());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_int_max, uint_type, UIntTypes)
{
  const uint_type x("0");
  const uint_type z = x + std::numeric_limits<int>::max();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<int>::max());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(subtract_signed_integral1, uint_type, UIntTypes)
{
  const uint_type x("987777");
  const uint_type z = x - 12345;
  BOOST_CHECK_EQUAL(z, "975432");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(subtract_signed_integral2, uint_type, UIntTypes)
{
  const uint_type x("98000");
  const uint_type z = x - (-1);
  BOOST_CHECK_EQUAL(z, "98001");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(subtract_signed_integral3, uint_type, UIntTypes)
{
  const uint_type x("125642682070");
  const long y = 2147483647;
  const uint_type z = x - y;
  BOOST_CHECK_EQUAL(z, "123495198423");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(subtract_unsigned_char1, uint_type, UIntTypes)
{
  const unsigned char y = 14;
  const uint_type x("987777");
  const uint_type z = x - y;
  BOOST_CHECK_EQUAL(z, "987763");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply_by_signed_integral1, uint_type, UIntTypes)
{
  const uint_type x("987777");
  const uint_type z = x * 12345;
  BOOST_CHECK_EQUAL(z, "12194107065");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply_by_unsigned_integral1, uint_type, UIntTypes)
{
  const uint_type x("1256");
  const uint_type z = x * 100U;
  uint_type w("125600");
  BOOST_CHECK_EQUAL(z, "125600");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply_by_zero1, uint_type, UIntTypes)
{
  const uint_type x("9877234234252377");
  const uint_type z = x * 0;
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply_by_negative_zero1, uint_type, UIntTypes)
{
  const uint_type x("9877234234252377");
  const uint_type z = x * -0;
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(divide_by_unsigned_char1, uint_type, UIntTypes)
{
  const unsigned char y = 16;
  const uint_type x("10000001");
  const uint_type z = x / y;
  BOOST_CHECK_EQUAL(z, "625000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(divide_by_unsigned_char2, uint_type, UIntTypes)
{
  const unsigned char y = 128;
  const uint_type x("14222200");
  const uint_type z = x / y;
  BOOST_CHECK_EQUAL(z, "111110");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(divide_by_signed_integral1, uint_type, UIntTypes)
{
  const uint_type x("786432");
  const uint_type z = x / 12;
  BOOST_CHECK_EQUAL(z, "65536");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modulo_signed_integral1, uint_type, UIntTypes)
{
  const uint_type x("786432");
  const uint_type z = x % 12;
  BOOST_CHECK_EQUAL(z, "0");
}

// TODO should this work?
//BOOST_AUTO_TEST_CASE_TEMPLATE(modulo_signed_integral3, uint_type, UIntTypes)
//{
//  const uint_type x("987777");
//  const uint_type z = x % -123456;
//  BOOST_CHECK_EQUAL(z, "129");
//}

BOOST_AUTO_TEST_CASE_TEMPLATE(modulo_unsigned_integral1, uint_type, UIntTypes)
{
  const uint_type x("987771");
  const uint_type z = x % 16U;
  BOOST_CHECK_EQUAL(z, "11");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bitwise_or_signed_integral1, uint_type, UIntTypes)
{
  const uint_type x("786432");
  const uint_type z = x | 1;
  BOOST_CHECK_EQUAL(z, "786433");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bitwise_or_unsigned_integral1, uint_type, UIntTypes)
{
  const uint_type x("786432");
  const uint_type z = x | 1U;
  BOOST_CHECK_EQUAL(z, "786433");
}

