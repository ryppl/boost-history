// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(bool1, uint_type, UIntTypes)
{
  // this just tests operator == (const uint_type&, bool)
  const uint_type x("1");
  const uint_type y("0");
  BOOST_CHECK_EQUAL(x, true);
  BOOST_CHECK_EQUAL(y, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bool_or, uint_type, UIntTypes)
{
  const uint_type x("1");
  const uint_type y("0");
  const bool z = x || y;
  BOOST_CHECK_EQUAL(z, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bool_and1, uint_type, UIntTypes)
{
  const uint_type x("1");
  const uint_type y("0");
  const bool z = x && y;
  BOOST_CHECK_EQUAL(z, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bool_and2, uint_type, UIntTypes)
{
  const uint_type x("1");
  const uint_type y("1");
  const bool z = x && y;
  BOOST_CHECK_EQUAL(z, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bool_loop_condition, uint_type, UIntTypes)
{
  uint_type x("5");
  while (x)
    --x;
  BOOST_CHECK_EQUAL(x, false);
}

