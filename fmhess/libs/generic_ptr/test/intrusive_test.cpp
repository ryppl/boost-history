//
//  intrusive_test.cpp
//
//  Copyright (c) 2009 Frank Mori Hess
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/generic_ptr/intrusive.hpp>
#include <boost/generic_ptr/asserting.hpp>
#include <boost/utility/enable_if.hpp>

namespace bgp = boost::generic_ptr;

namespace mynamespace
{
  class X
  {
  public:
    X():
      wrapped_count(0),
      pop_count(0)
    {}
    mutable unsigned wrapped_count;
    mutable unsigned pop_count;
  };
  void intrusive_ptr_add_ref(const X *x)
  {
    ++x->pop_count;
  };
  void intrusive_ptr_release(const X *x)
  {
    --x->pop_count;
  };
  template<typename T>
  void intrusive_ptr_add_ref(const bgp::asserting<T> &x,
    typename boost::enable_if<boost::is_convertible<T, const X*> >::type * = 0)
  {
    ++x->wrapped_count;
    intrusive_ptr_add_ref(get_pointer(x));
  };
  template<typename T>
  void intrusive_ptr_release(const bgp::asserting<T> &x,
    typename boost::enable_if<boost::is_convertible<T, const X*> >::type * = 0)
  {
    --x->wrapped_count;
    intrusive_ptr_release(get_pointer(x));
  };
}

void matroshka_add_ref_and_release_test()
{
  {
    mynamespace::X x;
    {
      bgp::intrusive<mynamespace::X*> xp(&x);
      BOOST_TEST(x.pop_count == 1);
      BOOST_TEST(x.wrapped_count == 0);
    }
    BOOST_TEST(x.pop_count == 0);
    BOOST_TEST(x.wrapped_count == 0);
  }
  {
    mynamespace::X x;
    {
      bgp::intrusive<bgp::asserting<mynamespace::X*> > xp(&x);
      BOOST_TEST(x.pop_count == 1);
      BOOST_TEST(x.wrapped_count == 1);
    }
    BOOST_TEST(x.pop_count == 0);
    BOOST_TEST(x.wrapped_count == 0);
  }
}

int main()
{
  matroshka_add_ref_and_release_test();
  return boost::report_errors();
}
