/* Boost.Flyweight test of set_factory.
 *
 * Copyright 2006-2007 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#include "test_set_factory.hpp"

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/flyweight/flyweight.hpp>
#include <boost/flyweight/refcounted.hpp>
#include <boost/flyweight/set_factory.hpp> 
#include <boost/flyweight/simple_locking.hpp>
#include <boost/flyweight/static_holder.hpp>
#include "lambda_components.hpp"
#include "test_basic_template.hpp"

using namespace boost::flyweights;

struct set_factory_flyweight_specifier1
{
  template<typename T>
  struct apply
  {
    typedef flyweight<T,set_factory<> > type;
  };
};

struct set_factory_flyweight_specifier2
{
  template<typename T>
  struct apply
  {
    typedef flyweight<
      T,
      static_holder_class<boost::mpl::_1>,
      set_factory_class<
        boost::mpl::_1,boost::mpl::_2,
        lambda_greater<boost::mpl::_2>
        
#if !defined(BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)
        ,std::allocator<boost::mpl::_1>
#endif

      >
    > type;
  };
};

struct set_factory_flyweight_specifier3
{
  template<typename T>
  struct apply
  {
    typedef flyweight<
      T,
      set_factory<
        lambda_greater<boost::mpl::_2>

#if !defined(BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)
        ,std::allocator<boost::mpl::_1>
#endif

      >,
      static_holder_class<boost::mpl::_1>,
      tag<char>
    > type;
  };
};

void test_set_factory()
{
  BOOST_FLYWEIGHT_TEST_BASIC(set_factory_flyweight_specifier1)
  BOOST_FLYWEIGHT_TEST_BASIC(set_factory_flyweight_specifier2)
  BOOST_FLYWEIGHT_TEST_BASIC(set_factory_flyweight_specifier3)
}
