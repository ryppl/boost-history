// Boost.Signals library

// Copyright Frank Mori Hess 2007
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/test/minimal.hpp>
#include <boost/thread_safe_signal.hpp>
#include <boost/bind.hpp>

struct swallow {
  typedef int result_type;
  template<typename T> result_type operator()(const T*, int i) { return i; }
};

template<typename T>
struct max_or_default {
  typedef T result_type;

  template<typename InputIterator>
  T operator()(InputIterator first, InputIterator last) const
  {
    if (first == last)
      return T();

    T max = *first++;
    for (; first != last; ++first)
      max = (*first > max)? *first : max;

    return max;
  }
};

static int myfunc(int i, double z)
{
  std::cout << __FUNCTION__ << ": i is " << i << " and z is " << z << std::endl;
  return i;
}

int test_main(int, char*[])
{
  typedef boost::signal1<int, int, max_or_default<int> > sig_type;
  sig_type s1;
  boost::signalslib::connection connection;

  // Test auto-disconnection
  BOOST_CHECK(s1(5) == 0);
  {
    boost::shared_ptr<int> shorty(new int());
    s1.connect(sig_type::slot_type(swallow(), shorty.get(), _1).track(shorty));
    BOOST_CHECK(s1(5) == 5);
  }
  BOOST_CHECK(s1(5) == 0);

  // Test auto-disconnection of slot before signal connection
  {
    boost::shared_ptr<int> shorty(new int(1));
// doesn't work on gcc 3.3.5, it says: error: type specifier omitted for parameter `shorty'
// does work on gcc 4.1.2
//    sig_type::slot_type slot(swallow(), shorty.get(), _1);
    swallow myswallow;
    sig_type::slot_type slot(myswallow, shorty.get(), _1);

    slot.track(shorty);
    shorty.reset();
    s1.connect(slot);
    BOOST_CHECK(s1(5) == 0);
  }

  // Test binding of a slot to another slot
  {
    boost::shared_ptr<int> shorty(new int(2));
    boost::slot<int (double)> other_slot(&myfunc, boost::cref(*shorty.get()), _1);
    other_slot.track(shorty);
    connection = s1.connect(sig_type::slot_type(other_slot, 0.5).track(other_slot));
    BOOST_CHECK(s1(3) == 2);
  }
  BOOST_CHECK(connection.connected() == false);
  BOOST_CHECK(s1(3) == 0);

  // Test binding of a signal as a slot
  {
    sig_type s2;
    s1.connect(sig_type::slot_type(s2));
    s2.connect(sig_type::slot_type(&myfunc, _1, 0.7));
    BOOST_CHECK(s1(4) == 4);
  }
  BOOST_CHECK(s1(4) == 0);
  return 0;
}
