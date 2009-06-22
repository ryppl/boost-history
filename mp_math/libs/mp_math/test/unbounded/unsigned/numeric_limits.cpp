// Copyright Kevin Sopp 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(is_signed, uint_type, UIntTypes)
{
  BOOST_CHECK_EQUAL(std::numeric_limits<uint_type>::is_signed, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(is_bounded, uint_type, UIntTypes)
{
  BOOST_CHECK_EQUAL(std::numeric_limits<uint_type>::is_bounded, false);
}

