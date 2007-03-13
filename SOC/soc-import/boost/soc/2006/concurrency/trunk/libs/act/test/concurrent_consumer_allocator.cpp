/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/detail/lightweight_test.hpp>

#include <boost/act/consumer_allocator/concurrent_consumer_allocator.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include "consumer_allocator_tester.hpp"

boost::mutex global_mutex;
boost::condition global_condition;

int scoped_consumer_value = 0;

struct test_fun
{
  test_fun( int& value_init ) : value( value_init ) {}

  void operator ()() const
  {
    boost::mutex::scoped_lock lock( global_mutex );
    value = 1;
    global_condition.notify_all();
  }

  int& value;
};

struct execute
{
  typedef void result_type;

  void operator ()( test_fun& target ) const
  {
    target();
  }
};

void test_scoped_consumer_allocator()
{
  using namespace boost::act;

  test_consumer_allocator< concurrent_consumer_allocator<> >();

  typedef concurrent_consumer_allocator<> allocator_type;
  typedef allocator_type::scoped_consumer< restraint< is< test_fun > >
                                         , processor< execute >
                                         >
                                         scoped_consumer_type;

  int test_value = 0;

  allocator_type::tracker<> tracker;

  {
    boost::mutex::scoped_lock lock( global_mutex );
    scoped_consumer_type consumer;

    tracker = consumer.tracked_consume( test_fun( scoped_consumer_value ) );

    global_condition.wait( lock );

    test_value = scoped_consumer_value;
  }

  tracker.wait();

  if( test_value != 1 )
    BOOST_ERROR( "concurrent_consumer_allocator scoped_consumer general execution failure." );
}

int main()
{
  test_scoped_consumer_allocator();

  return boost::report_errors();
}
