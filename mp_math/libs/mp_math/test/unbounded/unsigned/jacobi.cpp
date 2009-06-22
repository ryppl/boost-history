// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"


BOOST_AUTO_TEST_CASE_TEMPLATE(jacobi1, uint_type, UIntTypes)
{
  const uint_type x("1236");
  const uint_type y("20003");
  const int z = boost::mp_math::jacobi(x,y);
  BOOST_CHECK_EQUAL(z, 1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(jacobi2, uint_type, UIntTypes)
{
  const uint_type x("987897");
  const uint_type y("987");
  const int z = boost::mp_math::jacobi(x,y);
  BOOST_CHECK_EQUAL(z, 0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(jacobi3, uint_type, UIntTypes)
{
  const uint_type x("610");
  const uint_type y("987");
  const int z = boost::mp_math::jacobi(x,y);
  BOOST_CHECK_EQUAL(z, -1);
}
