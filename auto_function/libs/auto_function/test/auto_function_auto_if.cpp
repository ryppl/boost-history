/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/test/minimal.hpp>

#include <boost/array.hpp>
#include <boost/auto_function/auto_function.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/type_traits/is_same.hpp>

struct dummy_type {};

// For SFINAE check
dummy_type begin_of( ... );

template< class ContainerType >
BOOST_AUTO_FUNCTION
( ( begin_of( ContainerType& container_ ) )
, ( auto if boost::is_same< boost::mpl::_1, typename ContainerType::iterator > )
, ( return container_.begin() )
)

int test_main( int argc, char* argv[] )
{
  typedef boost::array< int, 1 > array_type;
  array_type        array_       = { { 0 } };
  array_type const& const_array_ = array_;

  // Check that type deduction is working properly
  BOOST_STATIC_ASSERT
  ( ( boost::is_same< decltype( ::begin_of( array_ ) )
                    , array_type::iterator
                    >::value
    )
  );

  // Check that SFINAE is working properly
  BOOST_STATIC_ASSERT
  ( ( boost::is_same< decltype( ::begin_of( 5 ) )
                    , dummy_type
                    >::value
    )
  );

  // Check that auto if is working properly
  BOOST_STATIC_ASSERT
  ( ( boost::is_same< decltype( ::begin_of( const_array_ ) )
                    , dummy_type
                    >::value
    )
  );

  BOOST_REQUIRE( ::begin_of( array_ ) == array_.begin() );

  return 0;
}
