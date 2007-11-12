/* Boost.Flyweight test of no_locking.
 *
 * Copyright 2006-2007 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#include "test_no_locking.hpp"

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/flyweight/flyweight.hpp>
#include <boost/flyweight/hashed_factory.hpp>
#include <boost/flyweight/no_locking.hpp> 
#include <boost/flyweight/refcounted.hpp>
#include <boost/flyweight/static_holder.hpp>
#include "test_basic_template.hpp"

using namespace boost::flyweights;

struct no_locking_flyweight_specifier
{
  template<typename T>
  struct apply
  {
    typedef flyweight<T,no_locking> type;
  };
};

void test_no_locking()
{
  BOOST_FLYWEIGHT_TEST_BASIC(no_locking_flyweight_specifier)
}
