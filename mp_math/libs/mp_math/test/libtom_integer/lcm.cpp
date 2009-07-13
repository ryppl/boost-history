// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"


BOOST_AUTO_TEST_CASE_TEMPLATE(lcm1, int_type, IntTypes)
{
  const int_type x("0");
  const int_type y("0");
  const int_type z = boost::mp_math::lcm(x,y);
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(lcm2, int_type, IntTypes)
{
  const int_type x("51111");
  const int_type y("0");
  const int_type z = boost::mp_math::lcm(x,y);
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(lcm3, int_type, IntTypes)
{
  const int_type x("4");
  const int_type y("6");
  const int_type z = boost::mp_math::lcm(x,y);
  BOOST_CHECK_EQUAL(z, "12");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(lcm4, int_type, IntTypes)
{
  const int_type x("-4");
  const int_type y("6");
  const int_type z = boost::mp_math::lcm(x,y);
  BOOST_CHECK_EQUAL(z, "12");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(lcm5, int_type, IntTypes)
{
  const int_type x("4");
  const int_type y("-6");
  const int_type z = boost::mp_math::lcm(x,y);
  BOOST_CHECK_EQUAL(z, "12");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(lcm6, int_type, IntTypes)
{
  const int_type x("-4");
  const int_type y("-6");
  const int_type z = boost::mp_math::lcm(x,y);
  BOOST_CHECK_EQUAL(z, "12");
}

#ifdef BOOST_HAS_VARIADIC_TMPL
BOOST_AUTO_TEST_CASE_TEMPLATE(variadic_lcm1, int_type, IntTypes)
{
  const int_type a("120");
  const int_type b("204");
  const int_type c("136");
  const int_type z = boost::mp_math::lcm(a,b,c);
  BOOST_CHECK_EQUAL(z, "2040");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(variadic_lcm2, int_type, IntTypes)
{
  const int_type a("12010");
  const int_type b("3299");
  const int_type c("84780");
  const int_type d("15");
  const int_type z = boost::mp_math::lcm(a,b,c,d);
  BOOST_CHECK_EQUAL(z, "335906753220");
}
#endif

