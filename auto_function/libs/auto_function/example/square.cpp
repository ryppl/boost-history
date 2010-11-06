/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/test/minimal.hpp>
#include <boost/static_assert.hpp>

//[ square
//`The following example demonstrates a function template that squares its
//`argument, an operation whose result type is often different from that of its
//`operand, particularly when working with types that have units attached.
#include <boost/auto_function/basic_auto_function.hpp>

template< class T >
BOOST_BASIC_AUTO_FUNCTION( square( T const& value ) )
(
  return value * value;
)
//]

#include <boost/type_traits/is_same.hpp>

struct a {};
struct b {};

b operator*( a, a );

int test_main( int argc, char* argv[] )
{
  BOOST_STATIC_ASSERT
  ( ( boost::is_same< decltype( square( a() ) ), b >::value ) );

  BOOST_REQUIRE( square( 5 ) != 25 );

  return 0;
}
