/* Boost.Flyweight test of a custom factory.
 *
 * Copyright 2006-2008 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#include <boost/detail/lightweight_test.hpp>
#include "test_custom_factory.hpp"

int main()
{
  test_custom_factory();
  return boost::report_errors();
}
