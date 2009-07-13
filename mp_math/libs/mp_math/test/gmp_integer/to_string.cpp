// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string1, int_type, IntTypes)
{
  const int_type x("0xabcdef123456789");
  const std::string s =
    x.template to_string<std::string>(std::ios::hex | std::ios::showbase);
  BOOST_CHECK_EQUAL(s, "0xabcdef123456789");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string2, int_type, IntTypes)
{
  const int_type x("12345678901234567890");
  const std::string s = x.template to_string<std::string>();
  BOOST_CHECK_EQUAL(s, "12345678901234567890");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string3, int_type, IntTypes)
{
  const int_type x("0xabcdef123456789");
  const std::string s = x.template to_string<std::string>(
      std::ios::hex | std::ios::showbase | std::ios::uppercase);
  BOOST_CHECK_EQUAL(s, "0XABCDEF123456789");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string4, int_type, IntTypes)
{
  const int_type x("76484675");
  const std::string s = x.template to_string<std::string>(std::ios::oct);
  BOOST_CHECK_EQUAL(s, "443610103");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string5, int_type, IntTypes)
{
  const int_type x("1024");
  const std::string s = x.template to_string<std::string>(std::ios::oct);
  BOOST_CHECK_EQUAL(s, "2000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string6, int_type, IntTypes)
{
  const int_type x("0");
  const std::string s =
    x.template to_string<std::string>(
        std::ios_base::dec | std::ios_base::showbase | std::ios_base::showpos);
  BOOST_CHECK_EQUAL(s, "+0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string7, int_type, IntTypes)
{
  const int_type x("0");
  const std::string s =
    x.template to_string<std::string>(
        std::ios_base::oct | std::ios_base::showbase | std::ios_base::showpos);
  BOOST_CHECK_EQUAL(s, "+0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string8, int_type, IntTypes)
{
  const int_type x("-0");
  const std::string s =
    x.template to_string<std::string>(
        std::ios_base::oct | std::ios_base::showbase | std::ios_base::showpos);
  BOOST_CHECK_EQUAL(s, "+0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string9, int_type, IntTypes)
{
  const int_type x("-1");
  const std::string s =
    x.template to_string<std::string>(
        std::ios_base::hex | std::ios_base::showbase | std::ios_base::showpos);
  BOOST_CHECK_EQUAL(s, "-0x1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string10, int_type, IntTypes)
{
  const int_type x("0x95a6801ce5292b9a8410e1a59dd29967");
  const std::string s =
    x.template to_string<std::string>(std::ios_base::hex);
  BOOST_CHECK_EQUAL(s, "95a6801ce5292b9a8410e1a59dd29967");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string11, int_type, IntTypes)
{
  const int_type x("0x12471fa56d6");
  const std::string s = x.template to_string<std::string>();
  BOOST_CHECK_EQUAL(s, "1256042682070");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string12, int_type, IntTypes)
{
  const int_type x("0x123456789abcdef");
  const std::string s = x.template to_string<std::string>(
      std::ios_base::hex | std::ios_base::showbase);
  BOOST_CHECK_EQUAL(s, "0x123456789abcdef");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(op_to_string13, int_type, IntTypes)
{
  const int_type x("0x123456789ABCDEF");
  const std::string s = x.template to_string<std::string>(
      std::ios_base::hex | std::ios_base::showbase | std::ios_base::uppercase);
  BOOST_CHECK_EQUAL(s, "0X123456789ABCDEF");
}

