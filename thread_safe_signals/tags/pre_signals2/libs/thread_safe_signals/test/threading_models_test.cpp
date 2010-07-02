// thread_safe_signals library
// basic test for alternate threading models

// Copyright Frank Mori Hess 2008
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#include <boost/test/minimal.hpp>
#include <boost/thread_safe_signal.hpp>
#include <boost/thread_safe_signals/multi_threaded.hpp>
#include <boost/thread_safe_signals/single_threaded.hpp>

// combiner that returns the number of slots invoked
struct slot_counter {
  typedef unsigned result_type;
  template<typename InputIterator>
  unsigned operator()(InputIterator first, InputIterator last) const
  {
    unsigned count = 0;
    for (; first != last; ++first)
    {
      try
      {
        *first;
        ++count;
      }
      catch(const boost::bad_weak_ptr &)
      {}
    }
    return count;
  }
};

void myslot()
{
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

template<typename signal_type>
void simple_test()
{
  signal_type sig;
  sig.connect(typename signal_type::slot_type(&myslot));
  BOOST_CHECK(sig() == 1);
  sig.disconnect(&myslot);
  BOOST_CHECK(sig() == 0);
}

int test_main(int, char*[])
{
  typedef boost::signal<void (), slot_counter, int, std::less<int>, boost::function<void ()>,
    boost::signalslib::multi_threaded> sig0_mt_type;
  simple_test<sig0_mt_type>();
  typedef boost::signal<void (), slot_counter, int, std::less<int>, boost::function<void ()>,
    boost::signalslib::single_threaded> sig0_st_type;
  simple_test<sig0_st_type>();
  return 0;
}