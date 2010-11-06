/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/test/minimal.hpp>

#include <boost/auto_function/auto_function.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/array.hpp>

struct dummy_type {};

// To check SFINAE
dummy_type begin_of( ... );

template< class T >
BOOST_AUTO_FUNCTION
( ( begin_of( T const& container_ ) )
, ( do container_.begin(); )
)

int test_main( int argc, char* argv[] )
{
  boost::array< int, 1 > array_ = { 0 };

  // Check that type deduction is working properly
  BOOST_STATIC_ASSERT
  ( ( boost::is_same< decltype( ::begin_of( array_ ) )
                    , boost::array< int, 1 >::const_iterator
                    >::value
    )
  );

  // Check that SFINAE is working properly
  BOOST_STATIC_ASSERT
  ( ( boost::is_same< decltype( ::begin_of( 0 ) )
                    , dummy_type
                    >::value
    )
  );

  // Verify that it is turning the correct value
  BOOST_REQUIRE( &array_.front() == &*::begin_of( array_ ) );

  return 0;
}
