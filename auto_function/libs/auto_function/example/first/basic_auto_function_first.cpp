/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

//[ basic_auto_function_first
#include <boost/auto_function/basic_auto_function.hpp>
#include <boost/range/begin.hpp>

template< class RangeType >
BOOST_BASIC_AUTO_FUNCTION( first( RangeType& range_ ) )
(
  return *boost::begin( range_ );
)
//]

#include <vector>
#include <boost/range/reference.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

std::vector< int > range_test;

BOOST_STATIC_ASSERT
( ( boost::is_same< decltype( ::first( range_test ) )
                  , boost::range_reference< std::vector< int > >::type
                  >::value
  )
);
