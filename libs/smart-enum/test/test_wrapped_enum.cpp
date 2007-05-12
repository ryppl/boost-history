//
// Test program to verify smart_enum<> and friends.
//
// Copyright (c) 2002-2007 Peter Simons <simons@cryp.to>
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "../../../boost/smart_enum.hpp"

#include <boost/static_assert.hpp>
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

enum myEnum { state1 = 5, state2, state3, state4 };

BOOST_AUTO_TEST_CASE( test_wrapped_enum )
{
  typedef boost::wrapped_smart_enum<myEnum, state1, state4> my_enum_t;
  my_enum_t e(state1);

  // Test the boundaries
  BOOST_STATIC_ASSERT((myEnum(my_enum_t::min) == state1));
  BOOST_STATIC_ASSERT((myEnum(my_enum_t::max) == state4));

  // Verify wrapping properties
  BOOST_REQUIRE_EQUAL(e, state1);
  BOOST_REQUIRE_EQUAL(++e, state2);
  BOOST_REQUIRE_EQUAL(++e, state3);
  BOOST_REQUIRE_EQUAL(e++, state3);
  BOOST_REQUIRE_EQUAL(e, state4);
  BOOST_REQUIRE_EQUAL(++e, state1);
  BOOST_REQUIRE_EQUAL((e += state4-state1+1), state1);
  BOOST_REQUIRE_EQUAL((e = state3 + (state4-state1+1)*2), state3);
  BOOST_REQUIRE_EQUAL((e += -10), state1);
}
