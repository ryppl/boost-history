/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_INTERLOCKED_BINARY_BITWISE_OPERATION_CHECKER_HPP
#define BOOST_ACT_TEST_INTERLOCKED_BINARY_BITWISE_OPERATION_CHECKER_HPP

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
#include <boost/utility/binary.hpp>

uint8_fast_t const binary_values[32] =
{
  BOOST_BINARY_U( 11010010 ), BOOST_BINARY_U( 00110110 )
, BOOST_BINARY_U( 11111111 ), BOOST_BINARY_U( 11010011 )
, BOOST_BINARY_U( 00000000 ), BOOST_BINARY_U( 10110100 )
, BOOST_BINARY_U( 01011110 ), BOOST_BINARY_U( 01011100 )
, BOOST_BINARY_U( 10110111 ), BOOST_BINARY_U( 00110111 )
, BOOST_BINARY_U( 00111010 ), BOOST_BINARY_U( 10110011 )
, BOOST_BINARY_U( 01101001 ), BOOST_BINARY_U( 01111010 )
, BOOST_BINARY_U( 11010110 ), BOOST_BINARY_U( 11110010 )
, BOOST_BINARY_U( 01011101 ), BOOST_BINARY_U( 11011011 )
, BOOST_BINARY_U( 01011111 ), BOOST_BINARY_U( 00011111 )
, BOOST_BINARY_U( 11010001 ), BOOST_BINARY_U( 10110100 )
, BOOST_BINARY_U( 01011100 ), BOOST_BINARY_U( 01011100 )
, BOOST_BINARY_U( 01101011 ), BOOST_BINARY_U( 01011101 )
, BOOST_BINARY_U( 11101011 ), BOOST_BINARY_U( 01101111 )
, BOOST_BINARY_U( 11110001 ), BOOST_BINARY_U( 10110100 )
, BOOST_BINARY_U( 10100010 ), BOOST_BINARY_U( 01110110 )
};

#undef BOOST_ACT_TEST_DETAIL_BASIC_BINARY_OP

template< typename ValueType, typename ResultType >
void check_operation_results( ValueType old_value, ValueType expected_value
                            , ResultType result
                            )
{
  ValueType const result_old_value = result.old_value(),
                  result_new_value = result.new_value();

  BOOST_CHECK( result_old_value == old_value );
  BOOST_CHECK( result_new_value == expected_value );
}


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

    check_operation_results( copy, single_thread_op( copy, val )
                                 , operation( test_var, val )
                           );
  }
}

#endif
