/* Boost.Flyweight test of no_locking.
 *
 * Copyright 2006-2007 Joaqu�n M L�pez Mu�oz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#include <boost/detail/lightweight_test.hpp>
#include "test_no_locking.hpp"

int main()
{
  test_no_locking();
  return boost::report_errors();
}
