//
//  throwing_test.cpp
//
//  Copyright (c) 2009 Frank Mori Hess
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/generic_ptr/throwing.hpp>

void implicit_conversion_test()
{
  int x;
  boost::generic_ptr::throwing<int*> tp;
  tp = &x;
  BOOST_TEST(tp == &x);
  int *p = tp;
  BOOST_TEST(p == tp);
  BOOST_TEST(p == &x);
}

class X
{
public:
  void f() {}
};

void throw_test()
{
  boost::generic_ptr::throwing<X*> tp(static_cast<X*>(0));
  try
  {
    *tp;
    BOOST_TEST(false);
  }catch(const std::invalid_argument &)
  {
  }
  try
  {
    tp->f();
    BOOST_TEST(false);
  }catch(const std::invalid_argument &)
  {
  }
}

int main()
{
  implicit_conversion_test();
  throw_test();
  return 0;
}
