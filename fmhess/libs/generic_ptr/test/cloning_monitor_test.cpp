//
//  cloning_monitor_test.cpp
//
//  Copyright (c) 2009 Frank Mori Hess
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/generic_ptr/cloning.hpp>
#include <boost/generic_ptr/monitor.hpp>

class event_counting_mutex
{
public:
  event_counting_mutex():
    lock_count(0),
    unlock_count(0),
    try_lock_count(0)
  {}
  void lock()
  {
    ++lock_count;
  }
  void unlock()
  {
    ++unlock_count;
  }
  bool try_lock()
  {
    ++lock_count;
    ++try_lock_count;
    return true;
  }
  unsigned lock_count;
  unsigned unlock_count;
  unsigned try_lock_count;
};

void clone_test()
{
  typedef boost::generic_ptr::monitor<int*, event_counting_mutex> monitor_type;
  typedef boost::generic_ptr::cloning<monitor_type> cloning_monitor_type;
  cloning_monitor_type p0(new int());
  BOOST_TEST(p0.get().get_mutex_ref().lock_count == 0);
  BOOST_TEST(p0.get().get_mutex_ref().unlock_count == 0);
  cloning_monitor_type p1(p0);
  BOOST_TEST(p0.get().get_mutex_ref().lock_count == 1);
  BOOST_TEST(p0.get().get_mutex_ref().unlock_count == 1);
  BOOST_TEST(p1.get().get_mutex_ref().lock_count == 0);
  BOOST_TEST(p1.get().get_mutex_ref().unlock_count == 0);
}

int main()
{
  clone_test();
  return boost::report_errors();
}
