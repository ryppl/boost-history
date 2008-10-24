// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"


BOOST_AUTO_TEST_CASE_TEMPLATE(gcd1, mp_int_type, mp_int_types)
{
  const mp_int_type x("10");
  const mp_int_type y("2");
  const mp_int_type z = boost::mp_math::gcd(x,y);
  BOOST_CHECK_EQUAL(z, "2");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(gcd2, mp_int_type, mp_int_types)
{
  const mp_int_type x("456489798");
  const mp_int_type y("987");
  const mp_int_type z = boost::mp_math::gcd(x,y);
  BOOST_CHECK_EQUAL(z, "3");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(gcd3, mp_int_type, mp_int_types)
{
  const mp_int_type x("-16384");
  const mp_int_type y("16384");
  const mp_int_type z = boost::mp_math::gcd(x,y);
  BOOST_CHECK_EQUAL(z, "16384");
}
