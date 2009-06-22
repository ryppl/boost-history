// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"


BOOST_AUTO_TEST_CASE_TEMPLATE(gcd1, uint_type, UIntTypes)
{
  const uint_type x("10");
  const uint_type y("2");
  const uint_type z = boost::mp_math::gcd(x,y);
  BOOST_CHECK_EQUAL(z, "2");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(gcd2, uint_type, UIntTypes)
{
  const uint_type x("456489798");
  const uint_type y("987");
  const uint_type z = boost::mp_math::gcd(x,y);
  BOOST_CHECK_EQUAL(z, "3");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(gcd3, uint_type, UIntTypes)
{
  const uint_type x("16384");
  const uint_type y("16384");
  const uint_type z = boost::mp_math::gcd(x,y);
  BOOST_CHECK_EQUAL(z, "16384");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(gcd4, uint_type, UIntTypes)
{
  const uint_type x("0");
  const uint_type y("0");
  const uint_type z = boost::mp_math::gcd(x,y);
  BOOST_CHECK_EQUAL(z, "0");
}

#ifdef BOOST_HAS_VARIADIC_TMPL
BOOST_AUTO_TEST_CASE_TEMPLATE(variadic_gcd1, uint_type, UIntTypes)
{
  const uint_type a("42");
  const uint_type b("56");
  const uint_type c("140");
  const uint_type z = boost::mp_math::gcd(a,b,c);
  BOOST_CHECK_EQUAL(z, "14");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(variadic_gcd2, uint_type, UIntTypes)
{
  const uint_type a("1200000000");
  const uint_type b("2400000000");
  const uint_type c("3600000000");
  const uint_type d("600000000000000");
  const uint_type z = boost::mp_math::gcd(a,b,c,d);
  BOOST_CHECK_EQUAL(z, "1200000000");
}
#endif

