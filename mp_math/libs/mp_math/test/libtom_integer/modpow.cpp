// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow1, int_type, IntTypes)
{
  const int_type x("2");
  const int_type exp("14");
  const int_type m("8");
  const int_type z = boost::mp_math::modpow(x, exp, m);
  BOOST_CHECK_EQUAL(z, "0");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow2, int_type, IntTypes)
{
  const int_type x("4");
  const int_type exp("13");
  const int_type m("497");
  const int_type z = boost::mp_math::modpow(x, exp, m);
  BOOST_CHECK_EQUAL(z, "445");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow3, int_type, IntTypes)
{
  const int_type x("2395422");
  const int_type exp("2424832");
  const int_type m("2424833");
  const int_type z = boost::mp_math::modpow(x, exp, m);
  BOOST_CHECK_EQUAL(z, "1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow4, int_type, IntTypes)
{
  const int_type x("184");
  const int_type exp("560");
  const int_type m("561");
  const int_type z = boost::mp_math::modpow(x, exp, m);
  BOOST_CHECK_EQUAL(z, "1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow5, int_type, IntTypes)
{
  const int_type x("997028168093060821869770104094480850560519901475");
  const int_type exp("7455602825647884208337395736200454918783366342656");
  const int_type m("7455602825647884208337395736200454918783366342657");
  const int_type z = boost::mp_math::modpow(x, exp, m);
  BOOST_CHECK_EQUAL(z, "1");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow6, int_type, IntTypes)
{
  const int_type x("184");
  const int_type exp("5600");
  const int_type m("2668");
  const int_type z = boost::mp_math::modpow(x, exp, m);
  BOOST_CHECK_EQUAL(z, "552");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow7, int_type, IntTypes)
{
  const int_type x("0x201abcff00aaffffffffffffffffffffffffffffffffffffffffff"
                   "ffffffffd6d7");
  const int_type exp("0x123456789abcdef01");
  // a modulus of type unrestricted diminished radix (2^253 - 41);
  const int_type m("0x1fffffffffffffffffffffffffffffffffffffffffffffffffffff"
                      "ffffffffd7");
  const int_type z = boost::mp_math::modpow(x, exp, m);
  BOOST_CHECK_EQUAL(z, "0x8f112a89871984cde410bb05621d5a6073557d2da0444b6681699"
                       "80b5ef825a");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow8, int_type, IntTypes)
{
  const int_type x("0x201abcff00aaffffffffffffffffffffffffffffffffffffffffff"
                   "ffffffffd6d7123456789abcdef01123456789abcdef01ffffffff");
  const int_type exp("0x123456789abcdef018978979899");
  // a modulus of type unrestricted diminished radix slow (2^503 - exp)
  const int_type m("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffff"
                   "fffffffffffffffffffffffffffffffffffffffffffffedcba987654"
                   "3210fe7687686767");
  const int_type z = boost::mp_math::modpow(x, exp, m);
  BOOST_CHECK_EQUAL(z, "0x1ada35751ae1f5fec7ab0e60f6c924d5f4a4a5d8f6786cdd78838"
                       "5ab16ebd994ee9aaea5faef3f490822ef443fd3e169caa3b608162e"
                       "01d40a593e775c9fa5");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow9, int_type, IntTypes)
{
  const int_type x("0x201abcff00aaffffffffffffffffffffffffffffffffffffffffff"
                   "ffffffffd6d7123456789abcdef01123456789abcdef01ffffffff");
  const int_type exp("0x123456789abcdef018978979899");
  // a modulus of type restricted diminished radix (2^256 - 53)
  const int_type m("0xffffffffffffffffffffffffffffffffffffffffffffffffffffff"
                   "ffffffffcb");
  const int_type z = boost::mp_math::modpow(x, exp, m);
  BOOST_CHECK_EQUAL(z, "0x777b5d9b290fbb5f99e4668cf1b0f723d3228fc252da492c54b75"
                       "8379f3024e4");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(modpow10, int_type, IntTypes)
{
  const int_type x("85219208314462076924446303193527165430");
  const int_type exp("99459745544104451690992478817018793139");
  const int_type m("198919491088208903381984957634037586279");
  const int_type z = boost::mp_math::modpow(x, exp, m);
  BOOST_CHECK_EQUAL(z, "1");
}

