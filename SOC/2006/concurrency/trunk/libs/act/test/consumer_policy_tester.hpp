/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_CONSUMER_POLICY_TESTER_HPP
#define BOOST_ACT_TEST_CONSUMER_POLICY_TESTER_HPP

#include <boost/detail/lightweight_test.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

#include <boost/type_traits/function_traits.hpp>

#include <boost/act/consumer_policy/consumer_arguments.hpp>

#include <boost/mpl/if.hpp>

#include <boost/act/interlocked/increment/increment_acquire.hpp>
#include <boost/act/interlocked/retrieve.hpp>
#include <boost/act/interlocked/integer/types.hpp>

#include <boost/act/consumer_policy/scoped_consumer.hpp>

#include <boost/shared_ptr.hpp>

struct execute
{
  typedef void result_type;

  template< typename FunType >
  void operator ()( FunType& target ) const
  {
    target();
  }
};

class long_process
{
public:
  long_process( boost::act::interlocked::int_fast32_t& target_init )
    : target_m( target_init ) {}

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

    boost::act::interlocked::increment_acquire( target_m );
  }

  int operator ()( int value ) const
  {
    (*this)();

    return value;
  }
private:
  boost::act::interlocked::int_fast32_t& target_m;
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
{/*
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
    BOOST_ERROR( "Consumer tracked result failure." );*/
}

template< typename ConsumerType >
void test_scoped_consumer_requirements( ConsumerType& consumer )
{
  //test_general_consumer_requirements( consumer );
}

template< typename ConsumerAllocator >
void test_no_arg_scoped_consumer()
{/*
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
    BOOST_ERROR( "Scoped consumer does not complete execution upon destruction." );*/
}

template< typename ConsumerPolicy >
void test_scoped_consumer()
{
  using namespace boost::act;

  interlocked::int_fast32_t test_val = 0;

  typedef typename consumer_policy::scoped_consumer
          <
            ConsumerPolicy
          , constraint< is< long_process > >
          , processor< execute >
          >
          ::type scoped_consumer_type;

  {
    scoped_consumer_type consumer;

    consumer( long_process( test_val ) );
    consumer( long_process( test_val ) );
  }

  if( interlocked::retrieve( test_val ) != 2 )
    BOOST_ERROR( "Scoped Consumer does not complete execution prior to." );
/*
  using namespace boost::act;

  typedef typename ConsumerAllocator
          ::template scoped_consumer< processor< long_process >
                                    , constraint< is< int > >
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
    BOOST_ERROR( "Scoped consumer tracked result failure." );*/
}

template< typename ConsumerAllocator >
void test_consumer_policy()
{
  test_scoped_consumer< ConsumerAllocator >();
  //test_no_arg_scoped_consumer< ConsumerAllocator >();
}

#endif
