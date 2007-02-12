/*=============================================================================
    Copyright (c) 2006-2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/detail/lightweight_test.hpp>

#include <boost/act/worker_allocator.hpp>

#include <boost/mpl/assert.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

boost::mutex global_mutex;
boost::condition global_condition;

int wild_worker_value = 0,
    unmanaged_worker_value = 0,
    safe_unmanaged_worker_value = 0;

struct worker_main_fun
{
  void operator ()() const
  {
    boost::mutex::scoped_lock lock( global_mutex );
    wild_worker_value = 1;
    global_condition.notify_all();
  }
};

void test_wild_worker()
{
  using namespace boost::act;

  typedef basic_worker_policy allocator_type;
  typedef allocator_type::unmanaged_worker unamanged_worker;

  allocator_type worker_allocator;

  int test_value = 0;

  {
    boost::mutex::scoped_lock lock( global_mutex );
    worker_allocator.spawn_wild_worker( worker_main_fun() );

    global_condition.wait( lock );

    test_value = wild_worker_value;
  }

  if( test_value != 1 )
    BOOST_ERROR( "basic_worker_allocator general thread execution failure." );
}

void test_unmanaged_worker() {}

void test_safe_unmanaged_worker() {}

int main()
{
  test_unmanaged_worker();
  test_safe_unmanaged_worker();
  test_wild_worker();

  return boost::report_errors();
}
