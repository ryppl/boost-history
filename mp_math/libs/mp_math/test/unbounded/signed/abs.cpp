// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(abs1, int_type, IntTypes)
{
  const int_type x("-0x123abdddfe4983");
  const int_type y = boost::mp_math::abs(x);
  BOOST_CHECK_EQUAL(y, "0x123abdddfe4983");
}

