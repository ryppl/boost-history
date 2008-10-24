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

BOOST_AUTO_TEST_CASE_TEMPLATE(pow_1, mp_int_type, mp_int_types)
{
  mp_int_type x("2");
  x.pow(0);
  BOOST_CHECK_EQUAL(x, "1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(pow_2, mp_int_type, mp_int_types)
{
  mp_int_type x("2");
  x.pow(1);
  BOOST_CHECK_EQUAL(x, "2");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(pow_3, mp_int_type, mp_int_types)
{
  mp_int_type x("2");
  x.pow(64);
  BOOST_CHECK_EQUAL(x, "18446744073709551616");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow1, mp_int_type, mp_int_types)
{
  mp_int_type x("2");
  const mp_int_type exp("14");
  const mp_int_type m("8");
  x.modpow(exp, m);
  BOOST_CHECK_EQUAL(x, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow2, mp_int_type, mp_int_types)
{
  mp_int_type x("4");
  const mp_int_type exp("13");
  const mp_int_type m("497");
  x.modpow(exp, m);
  BOOST_CHECK_EQUAL(x, "445");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow3, mp_int_type, mp_int_types)
{
  mp_int_type x("2395422");
  const mp_int_type exp("2424832");
  const mp_int_type m("2424833");
  x.modpow(exp, m);
  BOOST_CHECK_EQUAL(x, "1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow4, mp_int_type, mp_int_types)
{
  mp_int_type x("184");
  const mp_int_type exp("560");
  const mp_int_type m("561");
  x.modpow(exp, m);
  BOOST_CHECK_EQUAL(x, "1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow5, mp_int_type, mp_int_types)
{
  mp_int_type x("997028168093060821869770104094480850560519901475");
  const mp_int_type exp("7455602825647884208337395736200454918783366342656");
  const mp_int_type m("7455602825647884208337395736200454918783366342657");
  x.modpow(exp, m);
  BOOST_CHECK_EQUAL(x, "1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow6, mp_int_type, mp_int_types)
{
  mp_int_type x("184");
  const mp_int_type exp("5600");
  const mp_int_type m("2668");
  x.modpow(exp, m);
  BOOST_CHECK_EQUAL(x, "552");
}

