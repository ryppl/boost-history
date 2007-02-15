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

void test_scoped_consumer_allocator()
{
  using namespace boost::act;

  typedef concurrent_consumer_allocator<> allocator_type;
  typedef allocator_type::scoped_consumer<> consumer_type;

  int test_value = 0;

  {
    boost::mutex::scoped_lock lock( global_mutex );
    consumer_type consumer;

    consumer.call( test_fun( scoped_consumer_value ) );

    global_condition.wait( lock );

    test_value = scoped_consumer_value;
  }

  if( test_value != 1 )
    BOOST_ERROR( "basic_worker_allocator general thread execution failure." );
}

int main()
{
  test_scoped_consumer_allocator();

  return boost::report_errors();
}
