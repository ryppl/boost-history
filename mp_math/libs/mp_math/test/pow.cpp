// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(pow2_1, mp_int_type, mp_int_types)
{
  mp_int_type x;
  x.pow2(0);
  BOOST_CHECK_EQUAL(x, "1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(pow2_2, mp_int_type, mp_int_types)
{
  mp_int_type x;
  x.pow2(1);
  BOOST_CHECK_EQUAL(x, "2");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(pow2_3, mp_int_type, mp_int_types)
{
  mp_int_type x;
  x.pow2(64);
  BOOST_CHECK_EQUAL(x, "18446744073709551616");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(pow1, mp_int_type, mp_int_types)
{
  const mp_int_type x("2");
  const mp_int_type z = pow(x, 0);
  BOOST_CHECK_EQUAL(z, "1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(pow2, mp_int_type, mp_int_types)
{
  const mp_int_type x("2");
  const mp_int_type z = pow(x, 1);
  BOOST_CHECK_EQUAL(z, "2");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(pow3, mp_int_type, mp_int_types)
{
  const mp_int_type x("2");
  const mp_int_type z = pow(x, 64);
  BOOST_CHECK_EQUAL(z, "18446744073709551616");
}

