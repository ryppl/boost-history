/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_CONSUMER_ALLOCATOR_TESTER_HPP
#define BOOST_ACT_TEST_CONSUMER_ALLOCATOR_TESTER_HPP

#include <boost/detail/lightweight_test.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

#include <boost/type_traits/function_traits.hpp>

#include <boost/act/consumer_allocator/consumer_arguments.hpp>

struct long_process
{
  template< typename FunType >
  struct result
    : boost::mpl::if_c
      <
        ( boost::function_traits< FunType >::arity == 0 )
      , void
      , int
      >
  {
  };

  void operator ()() const
  {
    boost::xtime time_of_completion;

    boost::xtime_get( &time_of_completion, boost::TIME_UTC );

    time_of_completion.sec += 4;

    boost::thread::sleep( time_of_completion );
  }

  int operator ()( int value ) const
  {
    (*this)();

    return value;
  }
};

struct simple_process
{
  template< typename FunType >
  struct result
  {
    typedef typename boost::function_traits< FunType >::arg0_type type;
  };

  template< typename InputType >
  InputType operator ()( InputType const& value ) const
  {
    return value;
  }
};

template< typename ConsumerType >
void test_general_consumer_requirements( ConsumerType& consumer )
{
  using namespace boost::act;

  typedef typename ConsumerType
          ::template tracker_from_param< long >
          ::type tracker_type;

  consumer.consume( 4L );

  tracker_type const tracker6 = consumer.tracked_consume( 6L );

  consumer.consume( 10L );

  tracker_type const tracker32 = consumer.tracked_consume( 32L );

  tracker_type const tracker96 = consumer.tracked_consume( 96L );

  if(    ( tracker6.value() != 6L )
      || ( tracker32.value() != 32L )
      || ( tracker96.value() != 96L )
    )
    BOOST_ERROR( "Consumer tracked result failure." );
}

template< typename ConsumerType >
void test_scoped_consumer_requirements( ConsumerType& consumer )
{
  using namespace boost::act;

  typedef typename ConsumerAllocator
          ::template scoped_consumer< processor< simple_process >
                                    , restraint< is< long > >
                                    >
          consumer_type;

  consumer_type consumer;

  test_general_consumer_requirements( consumer );
}

template< typename ConsumerAllocator >
void test_no_arg_scoped_consumer()
{
  using namespace boost::act;

  typedef typename ConsumerAllocator
          ::template scoped_consumer< processor< long_process > >
          consumer_type;

  typedef typename consumer_type
          ::template tracker_from_param< void >
          ::type tracker_type;

  tracker_type tracker;

  {
    consumer_type consumer;

    tracker = consumer.tracked_consume();
  }

  if( !tracker )
    BOOST_ERROR( "Scoped consumer does not complete execution upon destruction." );
}

template< typename ConsumerAllocator >
void test_scoped_consumer()
{
  using namespace boost::act;

  typedef typename ConsumerAllocator
          ::template scoped_consumer< processor< long_process >
                                    , restraint< is< int > >
                                    >
          consumer_type;

  typedef typename consumer_type
          ::template tracker_from_param< int >
          ::type tracker_type;

  tracker_type tracker;

  {
    consumer_type consumer;

    tracker = consumer.tracked_consume( 8 );
  }

  if( !tracker )
    BOOST_ERROR( "Scoped consumer does not complete execution upon destruction." );

  if( tracker.value() != 8 )
    BOOST_ERROR( "Scoped consumer tracked result failure." );
}

template< typename ConsumerAllocator >
void test_consumer_allocator()
{
  test_scoped_consumer< ConsumerAllocator >();
  test_no_arg_scoped_consumer< ConsumerAllocator >();
}

#endif
