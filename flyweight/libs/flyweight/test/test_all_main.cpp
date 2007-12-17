/* Boost.Flyweight test suite.
 *
 * Copyright 2006-2007 Joaqu�n M L�pez Mu�oz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/detail/lightweight_test.hpp>
#include "test_assoc_cont_factory.hpp"
#include "test_basic.hpp"
#include "test_custom_factory.hpp"
#include "test_intermod_holder.hpp"
#include "test_init.hpp"
#include "test_multictor.hpp"
#include "test_no_locking.hpp"
#include "test_no_tracking.hpp"
#include "test_serialization.hpp"
#include "test_set_factory.hpp"

int main()
{
  test_assoc_container_factory();
  test_basic();
  test_custom_factory();
  test_init();

#if !defined(BOOST_MSVC)||!(BOOST_MSVC<1300)
  test_intermodule_holder();
#endif

  test_multictor();
  test_no_locking();
  test_no_tracking();

#if 0 /* #if'ed out til we get Boost.Serialization helper API back */
  test_serialization();
#endif

  test_set_factory();

  return boost::report_errors();
}
