// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(bool1, int_type, IntTypes)
{
  // this just tests operator == (const int_type&, bool)
  const int_type x("1");
  const int_type y("0");
  BOOST_CHECK_EQUAL(x, true);
  BOOST_CHECK_EQUAL(y, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bool_or, int_type, IntTypes)
{
  const int_type x("1");
  const int_type y("0");
  const bool z = x || y;
  BOOST_CHECK_EQUAL(z, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bool_and1, int_type, IntTypes)
{
  const int_type x("1");
  const int_type y("0");
  const bool z = x && y;
  BOOST_CHECK_EQUAL(z, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bool_and2, int_type, IntTypes)
{
  const int_type x("-1");
  const int_type y("-1");
  const bool z = x && y;
  BOOST_CHECK_EQUAL(z, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bool_loop_condition1, int_type, IntTypes)
{
  int_type x("5");
  while (x)
    --x;
  BOOST_CHECK_EQUAL(x, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bool_loop_condition2, int_type, IntTypes)
{
  int_type x("-5");
  while (x)
    ++x;
  BOOST_CHECK_EQUAL(x, false);
}

