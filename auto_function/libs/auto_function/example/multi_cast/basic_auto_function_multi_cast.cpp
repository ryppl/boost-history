/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

//[ basic_auto_function_multi_cast
// For BOOST_BASIC_AUTO_FUNCTION
#include <boost/auto_function/basic_auto_function.hpp>
#include <utility> // For std::forward

// The terminating case that returns the argument itself when no explicit
// target type is provided
template< class SourceType >
BOOST_BASIC_AUTO_FUNCTION( multi_cast( SourceType&& source ) )
(
  return source;
)

// The primary function template. It works by using a static_cast on the result
// of another multi_cast call with the final target type removed
template< class    TargetType
        , class... IntermediateTypes
        , class    SourceType
        >
BOOST_BASIC_AUTO_FUNCTION( multi_cast( SourceType&& source ) )
(
  return static_cast< TargetType >
         ( multi_cast< IntermediateTypes... >
           ( std::forward< SourceType >( source ) )
         );
)
//]

// Base types
struct left {};
struct right {};

// A derived type that inherits from both "left" and "right"
struct child : left, right {};

// A type unrelated to the above three types
struct unrelated_type {};

int main()
{
  // Create an instance of the derived type
  child child_;

  // Refer to the instance through its base type "left"
  left& left_ = child_;

  // The following line successfully cross-casts from "left_" to a reference to
  // the object through the type "right"
  right& right_ = multi_cast< right&, child& >( left_ );

  // Simple error message pointing to the following line
  right& error_ = multi_cast< right&, unrelated_type&, child& >( left_ );
}
