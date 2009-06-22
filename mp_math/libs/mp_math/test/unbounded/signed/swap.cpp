// Copyright Kevin Sopp 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"


BOOST_AUTO_TEST_CASE_TEMPLATE(swap1, int_type, IntTypes)
{
  int_type x("-0xabff23742384bf892734029323819048039");
  int_type y("0x1fee55d048039");

  boost::mp_math::swap(x, y);

  BOOST_CHECK_EQUAL(x, "0x1fee55d048039");
  BOOST_CHECK_EQUAL(y, "-0xabff23742384bf892734029323819048039");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(swap2, int_type, IntTypes)
{
  int_type x("-0xabff23742384bf892734029323819048039");
  int_type y("-0x1fee55d048039");

  x.swap(y);

  BOOST_CHECK_EQUAL(x, "-0x1fee55d048039");
  BOOST_CHECK_EQUAL(y, "-0xabff23742384bf892734029323819048039");
}

#ifdef BOOST_HAS_RVALUE_REFS
BOOST_AUTO_TEST_CASE_TEMPLATE(swap3, int_type, IntTypes)
{
  int_type x;
  int_type y;

  boost::mp_math::swap(int_type("-0x1fee55d048039"), x);
  boost::mp_math::swap(y, int_type("-0x1fee55d048039"));

  BOOST_CHECK_EQUAL(x, "-0x1fee55d048039");
  BOOST_CHECK_EQUAL(y, "-0x1fee55d048039");
}
#endif

#include <algorithm>

BOOST_AUTO_TEST_CASE_TEMPLATE(swap4, int_type, IntTypes)
{
  int_type x("-0xabff23742384bf892734029323819048039");
  int_type y("0x1fee55d048039");

  std::swap(x, y);

  BOOST_CHECK_EQUAL(x, "0x1fee55d048039");
  BOOST_CHECK_EQUAL(y, "-0xabff23742384bf892734029323819048039");
}

