// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(to_char1, uint_type, UIntTypes)
{
  uint_type x("123");
  const char z = x.template to_integral<char>();
  BOOST_CHECK_EQUAL(z, 123);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(to_int1, uint_type, UIntTypes)
{
  uint_type x("1023");
  const int z = x.template to_integral<int>();
  BOOST_CHECK_EQUAL(z, 1023);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(to_char_max, uint_type, UIntTypes)
{
  uint_type x(std::numeric_limits<char>::max());
  const int z = x.template to_integral<char>();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<char>::max());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(to_unsigned_char_min, uint_type, UIntTypes)
{
  uint_type x(std::numeric_limits<unsigned char>::min());
  const unsigned char z = x.template to_integral<unsigned char>();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<unsigned char>::min());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(to_unsigned_char_max, uint_type, UIntTypes)
{
  uint_type x(std::numeric_limits<unsigned char>::max());
  const unsigned char z = x.template to_integral<unsigned char>();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<unsigned char>::max());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(to_int_max, uint_type, UIntTypes)
{
  uint_type x(std::numeric_limits<int>::max());
  const int z = x.template to_integral<int>();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<int>::max());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(to_unsigned_int_min, uint_type, UIntTypes)
{
  uint_type x(std::numeric_limits<unsigned int>::min());
  const unsigned int z = x.template to_integral<unsigned int>();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<unsigned int>::min());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(to_unsigned_int_max, uint_type, UIntTypes)
{
  uint_type x(std::numeric_limits<unsigned int>::max());
  const unsigned int z = x.template to_integral<unsigned int>();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<unsigned int>::max());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(to_long_int_max, uint_type, UIntTypes)
{
  uint_type x(std::numeric_limits<long int>::max());
  const long int z = x.template to_integral<long int>();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<long int>::max());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(to_unsigned_long_int_min, uint_type, UIntTypes)
{
  uint_type x(std::numeric_limits<unsigned long int>::min());
  const unsigned long int z = x.template to_integral<unsigned long int>();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<unsigned long int>::min());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(to_unsigned_long_int_max, uint_type, UIntTypes)
{
  uint_type x(std::numeric_limits<unsigned long int>::max());
  const unsigned long int z = x.template to_integral<unsigned long int>();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<unsigned long int>::max());
}

#ifdef BOOST_HAS_LONG_LONG
BOOST_AUTO_TEST_CASE_TEMPLATE(to_long_long_int_max, uint_type, UIntTypes)
{
  uint_type x(std::numeric_limits<long long int>::max());
  const long long int z = x.template to_integral<long long int>();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<long long int>::max());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(to_unsigned_long_long_int_min, uint_type, UIntTypes)
{
  uint_type x(std::numeric_limits<unsigned long long int>::min());
  const unsigned long long int z = x.template to_integral<unsigned long long int>();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<unsigned long long int>::min());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(to_unsigned_long_long_int_max, uint_type, UIntTypes)
{
  uint_type x(std::numeric_limits<unsigned long long int>::max());
  const unsigned long long int z = x.template to_integral<unsigned long long int>();
  BOOST_CHECK_EQUAL(z, std::numeric_limits<unsigned long long int>::max());
}
#endif

