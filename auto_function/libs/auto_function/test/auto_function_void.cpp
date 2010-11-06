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

#include <vector>

struct dummy_type {};

// For SFINAE check
dummy_type push_back( ... );

template< class ContainerType >
BOOST_AUTO_FUNCTION
( ( push_back( ContainerType& container_ ) )
, ( void container_.push_back( 5 ) )
)

int test_main( int argc, char* argv[] )
{
  std::vector< int > vector_;

  // Check that type deduction is working properly
  BOOST_STATIC_ASSERT
  ( ( boost::is_same< decltype( ::push_back( vector_ ) )
                    , void
                    >::value
    )
  );

  // Check that SFINAE is working properly
  BOOST_STATIC_ASSERT
  ( ( boost::is_same< decltype( ::push_back( 5 ) )
                    , dummy_type
                    >::value
    )
  );

  ::push_back( vector_ );

  // Verify that it pushed back correctly
  BOOST_REQUIRE( vector_.size() == 1 && vector_.back() == 5 );

  return 0;
}
