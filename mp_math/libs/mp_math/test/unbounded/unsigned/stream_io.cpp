// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(dec_output, uint_type, UIntTypes)
{
  const uint_type x("1024");
  std::ostringstream os;
  os << x;
  BOOST_CHECK_EQUAL(os.str(), "1024");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(dec_output_w_showbase, uint_type, UIntTypes)
{
  const uint_type x("1024");
  std::ostringstream os;
  os.setf(std::ios_base::showbase);
  os << x;
  BOOST_CHECK_EQUAL(os.str(), "1024");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(oct_output, uint_type, UIntTypes)
{
  const uint_type x("1024");
  std::ostringstream os;
  os.setf(std::ios_base::oct, std::ios_base::basefield);
  os << x;
  BOOST_CHECK_EQUAL(os.str(), "2000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(oct_output_w_showbase, uint_type, UIntTypes)
{
  const uint_type x("1024");
  std::ostringstream os;
  os.setf(std::ios_base::oct, std::ios_base::basefield);
  os.setf(std::ios_base::showbase);
  os << x;
  BOOST_CHECK_EQUAL(os.str(), "02000");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(hex_output, uint_type, UIntTypes)
{
  const uint_type x("1024");
  std::ostringstream os;
  os.setf(std::ios_base::hex, std::ios_base::basefield);
  os << x;
  BOOST_CHECK_EQUAL(os.str(), "400");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(hex_output_w_showbase, uint_type, UIntTypes)
{
  const uint_type x("1024");
  std::ostringstream os;
  os.setf(std::ios_base::hex, std::ios_base::basefield);
  os.setf(std::ios_base::showbase);
  os << x;
  BOOST_CHECK_EQUAL(os.str(), "0x400");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(hex_output_w_showbase_and_uppercase, uint_type, UIntTypes)
{
  const uint_type x("0xabcdef0");
  std::ostringstream os;
  os.setf(std::ios_base::hex, std::ios_base::basefield);
  os.setf(std::ios_base::showbase | std::ios_base::uppercase);
  os << x;
  BOOST_CHECK_EQUAL(os.str(), "0XABCDEF0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(hex_output_w_showbase_and_showpos, uint_type, UIntTypes)
{
  const uint_type x("1024");
  std::ostringstream os;
  os.setf(std::ios_base::hex, std::ios_base::basefield);
  os.setf(std::ios_base::showbase | std::ios_base::showpos);
  os << x;
  BOOST_CHECK_EQUAL(os.str(), "+0x400");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(dec_input1, uint_type, UIntTypes)
{
  uint_type x;
  std::stringstream s;
  s << "123456";
  s >> x;
  BOOST_CHECK_EQUAL(x, "123456");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(dec_input2, uint_type, UIntTypes)
{
  uint_type x, y;
  std::stringstream s;
  s << "123456";
  s << " " << "987654321";
  s >> x;
  BOOST_CHECK_EQUAL(x, "123456");
  BOOST_REQUIRE(s.good());
  s >> y;
  BOOST_CHECK_EQUAL(y, "987654321");
  BOOST_CHECK(s.good());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(oct_input, uint_type, UIntTypes)
{
  uint_type x;
  std::stringstream s;
  s << "0123456";
  s >> x;
  BOOST_CHECK_EQUAL(x, "0123456");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(hex_input, uint_type, UIntTypes)
{
  uint_type x;
  std::stringstream s;
  s << "0xFFFFAB01";
  s >> x;
  BOOST_CHECK_EQUAL(x, "0xFFFFAB01");
}

