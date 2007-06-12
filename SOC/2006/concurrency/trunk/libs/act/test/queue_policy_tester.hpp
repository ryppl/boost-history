/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_QUEUE_POLICY_TESTER_HPP
#define BOOST_ACT_TEST_QUEUE_POLICY_TESTER_HPP

#include <boost/detail/lightweight_test.hpp>

#include <boost/act/queue_policy.hpp>

#include <string>
#include <typeinfo>

template< typename QueueType >
void test_queue()
{
  QueueType queue;

  std::string const name_of_type = typeid( queue ).name();

  if( queue.size() != 0 )
    BOOST_ERROR( ( "Initial size of " + name_of_type + " not 0." ).c_str() );

  if( !queue.empty() )
    BOOST_ERROR( ( "Initial size of " + name_of_type + " not 0." ).c_str() );

  queue.push( 1 );
  queue.push( 8 );
  queue.push( 2 );

  if( queue.size() != 3 )
    BOOST_ERROR( ( "Push failure with queue type:" + name_of_type + "." ).c_str() );

  if( queue.front() != 1 )
    BOOST_ERROR( ( "Front failure with queue type:" + name_of_type + "." ).c_str() );

  if( queue.back() != 2 )
    BOOST_ERROR( ( "Back failure with queue type:" + name_of_type + "." ).c_str() );

  queue.pop();

  if( queue.size() != 2 )
    BOOST_ERROR( ( "Pop failure with queue type:" + name_of_type + "." ).c_str() );

  if( queue.front() != 8 )
    BOOST_ERROR( ( "Front failure with queue type after pop:" + name_of_type + "." ).c_str() );

  if( queue.back() != 2 )
    BOOST_ERROR( ( "Back failure with queue type after pop:" + name_of_type + "." ).c_str() );

  queue.clear();

  if( queue.size() != 0 )
    BOOST_ERROR( ( "Size of " + name_of_type + " not 0 after clear." ).c_str() );

  if( !queue.empty() )
    BOOST_ERROR( ( "Size of " + name_of_type + " not 0 after clear." ).c_str() );
    
}

template< typename QueuePolicyType >
void test_safe_single_push_pop_queue()
{
  using namespace boost::act;

  typedef typename queue_policy
          ::safe_queue< QueuePolicyType, int
                      , one_pusher, one_popper
                      >
          ::type int_queue;

  test_queue< int_queue >();
};

template< typename QueuePolicyType >
void test_queue_policy()
{
  //test_queue();
  //test_safe_queue();
  //test_safe_single_push_queue();
  //test_safe_single_pop_queue();
  test_safe_single_push_pop_queue< QueuePolicyType >();
}

#endif
