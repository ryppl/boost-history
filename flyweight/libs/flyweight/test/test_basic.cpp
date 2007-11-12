/* Boost.Flyweight basic test.
 *
 * Copyright 2006-2007 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#include "test_basic.hpp"

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/flyweight.hpp> 
#include "lambda_components.hpp"
#include "test_basic_template.hpp"

using namespace boost::flyweights;

struct basic_flyweight_specifier1
{
  template<typename T>
  struct apply
  {
    typedef flyweight<T> type;
  };
};

struct basic_flyweight_specifier2
{
  template<typename T>
  struct apply
  {
    typedef flyweight<
      T,tag<int>,
      static_holder_class<boost::mpl::_1>,
      hashed_factory_class<
        boost::mpl::_1,boost::mpl::_2,
        lambda_hash<boost::mpl::_2>,lambda_equal_to<boost::mpl::_2>
        
#if !defined(BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)
        ,std::allocator<boost::mpl::_1>
#endif

      >,
      simple_locking,
      refcounted
    > type;
  };
};

struct basic_flyweight_specifier3
{
  template<typename T>
  struct apply
  {
    typedef flyweight<
      T,
      hashed_factory<
        lambda_hash<boost::mpl::_2>,lambda_equal_to<boost::mpl::_2>

#if !defined(BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)
        ,std::allocator<boost::mpl::_1>
#endif

      >,
      tag<int>
    > type;
  };
};

void test_basic()
{
  BOOST_FLYWEIGHT_TEST_BASIC(basic_flyweight_specifier1)
  BOOST_FLYWEIGHT_TEST_BASIC(basic_flyweight_specifier2)
  BOOST_FLYWEIGHT_TEST_BASIC(basic_flyweight_specifier3)
}
