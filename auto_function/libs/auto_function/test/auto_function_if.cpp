/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/test/minimal.hpp>

#include <boost/auto_function/auto_function.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

// For SFINAE check
struct variadic_result {};
struct auto_function_result {};

variadic_result arithmetic_function( ... );
variadic_result arithmetic_function_value_condition( ... );

template< class Type >
BOOST_AUTO_FUNCTION
( ( arithmetic_function( Type value_ ) )
, ( if boost::is_arithmetic< Type > )
, ( return auto_function_result() )
)

template< class Type >
BOOST_AUTO_FUNCTION
( ( arithmetic_function_value_condition( Type value_ ) )
, ( if boost::is_arithmetic< Type >::value )
, ( return auto_function_result() )
)

int test_main( int argc, char* argv[] )
{
  void* not_arithmetic_object = 0;

  // Check that type deduction is working properly
  BOOST_STATIC_ASSERT
  ( ( boost::is_same< decltype( arithmetic_function( 6. ) )
                    , auto_function_result
                    >::value
    )
  );

  // Check that type deduction is working properly
  BOOST_STATIC_ASSERT
  ( ( boost::is_same< decltype( arithmetic_function_value_condition( 6. ) )
                    , auto_function_result
                    >::value
    )
  );

  // Check that if is working properly
  BOOST_STATIC_ASSERT
  ( ( boost::is_same< decltype( arithmetic_function( not_arithmetic_object ) )
                    , variadic_result
                    >::value
    )
  );

  // Check that if is working properly
  BOOST_STATIC_ASSERT
  ( ( boost::is_same
      < decltype( arithmetic_function_value_condition( not_arithmetic_object ) )
      , variadic_result
      >::value
    )
  );

  return 0;
}
