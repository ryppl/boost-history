/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_INTERLOCKED_OPERATION_RESULT_CHECKER_HPP
#define BOOST_ACT_TEST_INTERLOCKED_OPERATION_RESULT_CHECKER_HPP

/*
Note: The template in this file only checks for proper result functionality.
      It is not a multithreaded test for checking the application of interlocked
      operations in a multithreaded environment.
*/

#include <boost/test/minimal.hpp>
#include <boost/foreach.hpp>
#include <boost/act/interlocked/integer/types.hpp>

#include "../random_uint8.hpp"

#include <boost/preprocessor/cat.hpp>

#define BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP( name, op )                      \
struct BOOST_PP_CAT( basic_, name )                                            \
{                                                                              \
  template< typename LeftType, typename RightType >                            \
  LeftType operator ()( LeftType const& left, RightType const& right ) const   \
  {                                                                            \
    return static_cast< LeftType >( left op right );                           \
  }                                                                            \
};

BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP( add,      + )
BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP( subtract, - )
BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP( multiply, * )
BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP( divide,   / )
BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP( mod,      % )
BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP( bitand,   & )
BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP( bitor,    | )
BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP( xor,      ^ )
BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP( lshift,   << )
BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP( rshift,   >> )

#undef BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP

template< typename SingleThreadOperationType, typename OperationType >
void brute_operation_result_checker( SingleThreadOperationType single_thread_op
                                   , OperationType operation
                                   )
{
  typedef boost::act::interlocked::uint_least8_t  uint_least8_t;

  uint_least8_t test_var = 0;

  BOOST_FOREACH( uint_least8_t val, random_uint8 )
  {
    uint_least8_t const copy = test_var;

    BOOST_CHECK( operation( test_var, val ) == copy );
  }
}

#endif
