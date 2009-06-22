// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/test/unit_test.hpp>
#include "prerequisite.hpp"
#include <boost/mp_math/integer_serialization.hpp>

BOOST_AUTO_TEST_CASE_TEMPLATE(test_serialization1, uint_type, UIntTypes)
{
  uint_type x("0x123456789abcdef25700000000003a");
  uint_type y;

  std::stringstream s;

  // Wrap this in a block so that the text_oarchive dtor writes a terminating
  // null character to the stream before opening an input archive on it.
  // See the note on the stream_error exception in the Boost.Serialization
  // documentation.
  {
    boost::archive::text_oarchive oa(s);
    oa << x;
  }

  {
    boost::archive::text_iarchive ia(s);
    ia >> y;
  }

  BOOST_CHECK_EQUAL(x, y);
}

