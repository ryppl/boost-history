// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"


BOOST_AUTO_TEST_CASE_TEMPLATE(lcm1, uint_type, UIntTypes)
{
  const uint_type x("0");
  const uint_type y("0");
  const uint_type z = boost::mp_math::lcm(x,y);
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(lcm2, uint_type, UIntTypes)
{
  const uint_type x("51111");
  const uint_type y("0");
  const uint_type z = boost::mp_math::lcm(x,y);
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(lcm3, uint_type, UIntTypes)
{
  const uint_type x("4");
  const uint_type y("6");
  const uint_type z = boost::mp_math::lcm(x,y);
  BOOST_CHECK_EQUAL(z, "12");
}

#ifdef BOOST_HAS_VARIADIC_TMPL
BOOST_AUTO_TEST_CASE_TEMPLATE(variadic_lcm1, uint_type, UIntTypes)
{
  const uint_type a("120");
  const uint_type b("204");
  const uint_type c("136");
  const uint_type z = boost::mp_math::lcm(a,b,c);
  BOOST_CHECK_EQUAL(z, "2040");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(variadic_lcm2, uint_type, UIntTypes)
{
  const uint_type a("12010");
  const uint_type b("3299");
  const uint_type c("84780");
  const uint_type d("15");
  const uint_type z = boost::mp_math::lcm(a,b,c,d);
  BOOST_CHECK_EQUAL(z, "335906753220");
}
#endif

