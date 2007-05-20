/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/detail/lightweight_test.hpp>

#include <boost/act/consumer_policy/concurrent_consumer_policy.hpp>
#include <boost/act/consumer_policy/is.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include "consumer_policy_tester.hpp"

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

void test_scoped_concurrent_consumer_policy()
{
  using namespace boost::act;

  test_consumer_policy< concurrent_consumer_policy<> >();

  //test_consumer_policy< concurrent_consumer_policy<> >();

  typedef concurrent_consumer_policy<> policy_type;

  typedef consumer_policy::scoped_consumer< policy_type
                                          , constraint< is< test_fun > >
                                          , processor< execute >
                                          >
                                          ::type scoped_consumer_type;

  int old_value,
      new_value;

  {
    boost::mutex::scoped_lock lock( global_mutex );
    scoped_consumer_type consumer;

    consumer( test_fun( scoped_consumer_value ) );

    old_value = scoped_consumer_value;

    global_condition.wait( lock );

    new_value = scoped_consumer_value;
  }

  if( old_value != 0 || new_value != 1 )
    BOOST_ERROR( "concurrent_consumer_policy scoped_consumer general execution failure." );
}

int main()
{
  test_scoped_concurrent_consumer_policy();

  return boost::report_errors();
}
