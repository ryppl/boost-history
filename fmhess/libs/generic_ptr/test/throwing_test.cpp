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

int main()
{
  implicit_conversion_test();
  return 0;
}
