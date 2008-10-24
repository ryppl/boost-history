// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(sqrt1, mp_int_type, mp_int_types)
{
  const mp_int_type x("279841");
  const mp_int_type y = sqrt(x);
  BOOST_CHECK_EQUAL(y, "529");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(sqrt2, mp_int_type, mp_int_types)
{
  const mp_int_type x("78310985281");
  const mp_int_type y = sqrt(x);
  BOOST_CHECK_EQUAL(y, "279841");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(nth_root1, mp_int_type, mp_int_types)
{
  const mp_int_type x("85766121");
  const mp_int_type y = nth_root(x, 3);
  BOOST_CHECK_EQUAL(y, "441");
}

