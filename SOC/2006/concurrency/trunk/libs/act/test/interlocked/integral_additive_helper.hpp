/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_INTERLOCKED_INTEGRAL_ADDITIVE_HELPER_HPP
#define BOOST_ACT_TEST_INTERLOCKED_INTEGRAL_ADDITIVE_HELPER_HPP

#include <boost/test/minimal.hpp>
#include <boost/foreach.hpp>
#include <boost/act/interlocked/integer/types.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/integer_traits.hpp>

#include "../random_uint8.hpp"

/*
Logic for additive tests:

Initialize an unsigned value.

Create a table with N random values.

Create N threads, with each one an assigned value from the table.

All threads approach a barrier after which they all perform their operation

Join all threads.

Compare new value with the value it should be.

*/

struct basic_add_assign
{
  template< typename LeftType, typename RightType >
  void operator ()( LeftType& left, RightType const& right ) const
  {
    left += right;
  }
};

struct basic_subtract_assign
{
  template< typename LeftType, typename RightType >
  void operator ()( LeftType& left, RightType const& right ) const
  {
    left -= right;
  }
};

template< typename BarrierType, typename OperationType
        , typename DestinationType, typename OperandType
        >
class perform_operation
{
public:
  perform_operation( BarrierType& barrier_init, OperationType operation_init
                   , DestinationType& destination_init, OperandType operand_init
                   )
    : barrier_m( barrier_init ), operation_m( operation_init )
    , destination_m( destination_init ), operand_m( operand_init ) {}
public:
  void operator ()() const
  {
    barrier_m.wait(); // Wait until all other perform_operation are ready

    operation_m( destination_m, operand_m );
  }
private:
  BarrierType& barrier_m;
  OperationType operation_m;
  DestinationType& destination_m;
  OperandType operand_m;
};

template< typename BarrierType, typename OperationType
        , typename DestinationType, typename OperandType
        >
perform_operation< BarrierType, OperationType
                 , DestinationType, OperandType
                 >
make_perform_operation( BarrierType& barrier, OperationType operation
                      , DestinationType& destination, OperandType operand
                      )
{
  return perform_operation< BarrierType, OperationType
                          , DestinationType, OperandType
                          >
                          ( barrier, operation
                          , destination, operand
                          );
}

template< typename SingleThreadOperationType, typename OperationType >
void brute_additive_assign_test( SingleThreadOperationType single_thread_op
                               , OperationType operation
                               )
{
  boost::barrier barrier( static_cast< unsigned int >( random_uint8.size() ) );

  boost::thread_group thread_group;

  typedef boost::act::interlocked::uint_least8_t  uint_least8_t;
  typedef boost::act::interlocked::uint_least32_t uint_least32_t;

  uint_least32_t test_var
    = boost::integer_traits< uint_least32_t >::const_max / 2,
                 test_var_copy = test_var;

  BOOST_FOREACH( uint_least8_t val, random_uint8 )
  {
    thread_group.add_thread
      ( new boost::thread( make_perform_operation( barrier, operation
                                                 , test_var, val
                                                 )
                         )
      );
  }

  thread_group.join_all();

  BOOST_FOREACH( uint_least8_t val, random_uint8 )
  {
    single_thread_op( test_var_copy, val );
  }

  BOOST_CHECK( boost::act::interlocked::retrieve( test_var ) == test_var_copy );
}

#endif
