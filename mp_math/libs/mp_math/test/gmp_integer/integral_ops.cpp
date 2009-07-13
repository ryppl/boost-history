// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(construct_from_zero, int_type, IntTypes)
{
  const int_type x(0);
  BOOST_CHECK_EQUAL(!x, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_signed_char_min, int_type, IntTypes)
{
  const signed char x = std::numeric_limits<signed char>::min();
  const int_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_signed_char_max, int_type, IntTypes)
{
  const signed char x = std::numeric_limits<signed char>::max();
  const int_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_unsigned_char_min, int_type, IntTypes)
{
  const unsigned char x = std::numeric_limits<unsigned char>::min();
  const int_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_unsigned_char_max, int_type, IntTypes)
{
  const unsigned char x = std::numeric_limits<unsigned char>::max();
  const int_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_int_min, int_type, IntTypes)
{
  const int x = std::numeric_limits<int>::min();
  const int_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_int_max, int_type, IntTypes)
{
  const int x = std::numeric_limits<int>::max();
  const int_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_unsigned_int_min, int_type, IntTypes)
{
  const unsigned int x = std::numeric_limits<unsigned int>::min();
  const int_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_unsigned_int_max, int_type, IntTypes)
{
  const unsigned int x = std::numeric_limits<unsigned int>::max();
  const int_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

#ifdef BOOST_HAS_LONG_LONG
BOOST_AUTO_TEST_CASE_TEMPLATE(equal_long_long_int_min, int_type, IntTypes)
{
  const long long int x = std::numeric_limits<long long int>::min();
  const int_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_long_long_int_max, int_type, IntTypes)
{
  const long long int x = std::numeric_limits<long long int>::max();
  const int_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_unsigned_long_long_int_min, int_type, IntTypes)
{
  const unsigned long long int x = std::numeric_limits<unsigned long long int>::min();
  const int_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(equal_unsigned_long_long_int_max, int_type, IntTypes)
{
  const unsigned long long int x = std::numeric_limits<unsigned long long int>::max();
  const int_type y(x);
  BOOST_CHECK_EQUAL(x, y);
}
#endif

BOOST_AUTO_TEST_CASE_TEMPLATE(add_signed_integral1, int_type, IntTypes)
{
  const int_type x("987777");
  const int_type z = x + 1;
  BOOST_CHECK_EQUAL(z, "987778");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_signed_integral2, int_type, IntTypes)
{
  const int_type x("987777");
  const int_type z = x + (-1);
  BOOST_CHECK_EQUAL(z, "987776");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_signed_integral3, int_type, IntTypes)
{
  const int_type x("9");
  const int_type z = x + (-10);
  BOOST_CHECK_EQUAL(z, "-1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_signed_integral4, int_type, IntTypes)
{
  const int_type x("-1");
  const int_type z = x + 5;
  BOOST_CHECK_EQUAL(z, "4");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_unsigned_integral1, int_type, IntTypes)
{
  const int_type x("9999999");
  const int_type z = x + 1U;
  BOOST_CHECK_EQUAL(z, "10000000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_signed_char_min, int_type, IntTypes)
{
  const int_type x("0");
  const int_type z = x + std::numeric_limits<signed char>::min();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<signed char>::min());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_signed_char_max, int_type, IntTypes)
{
  const int_type x("0");
  const int_type z = x + std::numeric_limits<signed char>::max();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<signed char>::max());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_int_min, int_type, IntTypes)
{
  const int_type x("0");
  const int_type z = x + std::numeric_limits<int>::min();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<int>::min());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_int_max, int_type, IntTypes)
{
  const int_type x("0");
  const int_type z = x + std::numeric_limits<int>::max();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<int>::max());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(subtract_signed_integral1, int_type, IntTypes)
{
  const int_type x("987777");
  const int_type z = x - 12345;
  BOOST_CHECK_EQUAL(z, "975432");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(subtract_signed_integral2, int_type, IntTypes)
{
  const int_type x("98000");
  const int_type z = x - (-1);
  BOOST_CHECK_EQUAL(z, "98001");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(subtract_signed_integral3, int_type, IntTypes)
{
  const int_type x("125642682070");
  const long y = 2147483647;
  const int_type z = x - y;
  BOOST_CHECK_EQUAL(z, "123495198423");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(subtract_signed_integral4, int_type, IntTypes)
{
  const int_type x("1");
  const int_type z = x - 2;
  BOOST_CHECK_EQUAL(z, "-1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(subtract_signed_integral5, int_type, IntTypes)
{
  const int_type x("-0x23489328938ababba000909909");
  const int_type z = x - (-2);
  BOOST_CHECK_EQUAL(z, "-0x23489328938ababba000909907");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(subtract_signed_integral6, int_type, IntTypes)
{
  const int_type x("-0x23489328938ababba000909909");
  const int_type z = x - 0x200;
  BOOST_CHECK_EQUAL(z, "-0x23489328938ababba000909b09");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(subtract_unsigned_char1, int_type, IntTypes)
{
  const unsigned char y = 14;
  const int_type x("987777");
  const int_type z = x - y;
  BOOST_CHECK_EQUAL(z, "987763");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply1, int_type, IntTypes)
{
  const int_type x("-14");
  const signed char y = 100;
  const int_type z = x * y;
  BOOST_CHECK_EQUAL(z, "-1400");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply2, int_type, IntTypes)
{
  const int_type x("-14");
  const int y = 10000;
  const int_type z = x * y;
  BOOST_CHECK_EQUAL(z, "-140000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply3, int_type, IntTypes)
{
  const int_type x("-14");
  const signed char y = -100;
  const int_type z = x * y;
  BOOST_CHECK_EQUAL(z, "1400");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply4, int_type, IntTypes)
{
  const int_type x("-14");
  const int y = -10000;
  const int_type z = x * y;
  BOOST_CHECK_EQUAL(z, "140000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply5, int_type, IntTypes)
{
  const int_type x("-14");
  const unsigned char y = 100;
  const int_type z = x * y;
  BOOST_CHECK_EQUAL(z, "-1400");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply6, int_type, IntTypes)
{
  const int_type x("-14");
  const unsigned int y = 10000;
  const int_type z = x * y;
  BOOST_CHECK_EQUAL(z, "-140000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply7, int_type, IntTypes)
{
  const int_type x("-987777");
  const signed char y = 123;
  const int_type z = x * y;
  BOOST_CHECK_EQUAL(z, "-121496571");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply8, int_type, IntTypes)
{
  const int_type x("-987777");
  const int y = 12345;
  const int_type z = x * y;
  BOOST_CHECK_EQUAL(z, "-12194107065");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply9, int_type, IntTypes)
{
  const int_type x("987777");
  const int_type z = x * -12345;
  BOOST_CHECK_EQUAL(z, "-12194107065");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply10, int_type, IntTypes)
{
  const int_type x("-987777");
  const int_type z = x * -12345;
  BOOST_CHECK_EQUAL(z, "12194107065");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply11, int_type, IntTypes)
{
  const int_type x("1256");
  const int_type z = x * 100U;
  int_type w("125600");
  BOOST_CHECK_EQUAL(z, "125600");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply12, int_type, IntTypes)
{
  const int_type x("-0x500001f0000000");
  const unsigned long y = 0x413000;
  const int_type z = x * y;
  BOOST_CHECK_EQUAL(z, "-0x145f007e4d0000000000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply_by_zero1, int_type, IntTypes)
{
  const int_type x("-1");
  const int y = 0;
  const int_type z = x * y;
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply_by_zero2, int_type, IntTypes)
{
  const int_type x("-9877234234252377");
  const int y = 0;
  const int_type z = x * y;
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply_by_zero3, int_type, IntTypes)
{
  const int_type x("-9877234234252377");
  const unsigned int y = 0;
  const int_type z = x * y;
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(multiply_by_negative_zero1, int_type, IntTypes)
{
  const int_type x("-9877234234252377");
  const int_type z = x * -0;
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(divide_by_unsigned_char1, int_type, IntTypes)
{
  const unsigned char y = 16;
  const int_type x("10000001");
  const int_type z = x / y;
  BOOST_CHECK_EQUAL(z, "625000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(divide_by_unsigned_char2, int_type, IntTypes)
{
  const unsigned char y = 128;
  const int_type x("14222200");
  const int_type z = x / y;
  BOOST_CHECK_EQUAL(z, "111110");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(divide_by_signed_integral1, int_type, IntTypes)
{
  const int_type x("786432");
  const int_type z = x / 12;
  BOOST_CHECK_EQUAL(z, "65536");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modulo_signed_integral1, int_type, IntTypes)
{
  const int_type x("786432");
  const int_type z = x % 12;
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modulo_signed_integral2, int_type, IntTypes)
{
  const int_type x("-987777");
  const int_type z = x % 123456;
  BOOST_CHECK_EQUAL(z, "-129");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modulo_signed_integral3, int_type, IntTypes)
{
  const int_type x("987777");
  const int_type z = x % -123456;
  BOOST_CHECK_EQUAL(z, "129");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modulo_signed_integral4, int_type, IntTypes)
{
  const int_type x("-987777");
  const int_type z = x % -123456;
  BOOST_CHECK_EQUAL(z, "-129");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modulo_unsigned_integral1, int_type, IntTypes)
{
  const int_type x("987771");
  const int_type z = x % 16U;
  BOOST_CHECK_EQUAL(z, "11");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modulo_unsigned_integral2, int_type, IntTypes)
{
  const int_type x("-987771");
  const int_type z = x % 16U;
  BOOST_CHECK_EQUAL(z, "-11");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bitwise_or_signed_integral1, int_type, IntTypes)
{
  const int_type x("786432");
  const int_type z = x | 1;
  BOOST_CHECK_EQUAL(z, "786433");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bitwise_or_signed_integral2, int_type, IntTypes)
{
  const int_type x("786432");
  const int_type z = x | -1;
  BOOST_CHECK_EQUAL(z, "-786433");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bitwise_or_unsigned_integral1, int_type, IntTypes)
{
  const int_type x("786432");
  const int_type z = x | 1U;
  BOOST_CHECK_EQUAL(z, "786433");
}

