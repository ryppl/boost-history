/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

//[ no_auto_function_first
#include <boost/range/begin.hpp>     // For boost::begin
#include <boost/range/reference.hpp> // For the range_reference metafunction

// Dereference the first element of the range
template< class RangeType >
typename boost::range_reference< RangeType >::type
first( RangeType& range_ )
{
  return *boost::begin( range_ );
}
//]
