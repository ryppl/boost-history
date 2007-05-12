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

#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/if.hpp>
#include <boost/lambda/switch.hpp>
#include <boost/lambda/loops.hpp>
#include <boost/lambda/casts.hpp>
#include <boost/lambda/numeric.hpp>
#include <boost/lambda/exceptions.hpp>
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

enum myEnum { north = 17, east = 4, south = 92, west = -5 };

BOOST_AUTO_TEST_CASE( test_lambda_smart_enum )
{
  using namespace boost::lambda;
  typedef boost::smart_enum< myEnum, boost::function<myEnum (int, int)> > my_enum_t;

  my_enum_t e
    (  north
    ,  (
          // Guarantee that _1 is a valid myEnum.
          if_then( _1 != north && _1 != east && _1 != south && _1 != west
                 , throw_exception(std::out_of_range("invalid myEnum!"))
                 ),

          // Use the knowledge that myEnum is wrapped to short-cut increments.
          _2 = _2 % 4,
          if_then(_2 < 0, _2 = 4 + _2),
          while_loop( --_2 >= 0
                    , switch_statement( _1
                                      , case_statement<north>(_1 = east)
                                      , case_statement<east> (_1 = south)
                                      , case_statement<south>(_1 = west)
                                      , case_statement<west> (_1 = north)
                                      )
                    ),

          // Return result.
          ll_static_cast<myEnum>(_1)
       )
    );

  // Test operator++.
  BOOST_REQUIRE_EQUAL(e, north);
  BOOST_REQUIRE_EQUAL(++e, east);
  BOOST_REQUIRE_EQUAL(++e, south);
  BOOST_REQUIRE_EQUAL(++e, west);
  BOOST_REQUIRE_EQUAL(++e, north);
  BOOST_REQUIRE_EQUAL(++e, east);

  // Test operator--.
  BOOST_REQUIRE_EQUAL(--e, north);
  BOOST_REQUIRE_EQUAL(--e, west);
  BOOST_REQUIRE_EQUAL(--e, south);
  BOOST_REQUIRE_EQUAL(--e, east);
  BOOST_REQUIRE_EQUAL(--e, north);

  // Test operator+= and operator-=.
  BOOST_REQUIRE_EQUAL((e += 1), east);
  BOOST_REQUIRE_EQUAL((e -= 2), west);
  BOOST_REQUIRE_EQUAL((e -= 8), west);
  BOOST_REQUIRE_EQUAL((e += 7), south);

  // Test assignment.
  BOOST_REQUIRE_EQUAL((e = -5), west);
  BOOST_REQUIRE_THROW(e = 0, std::out_of_range);
  BOOST_REQUIRE_EQUAL(e, west);
  BOOST_REQUIRE_EQUAL((e = 80 + 12), south);
  BOOST_REQUIRE_EQUAL((e = e / 23), east);
}
