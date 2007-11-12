/* Boost.Flyweight test of assoc_container_factory.
 *
 * Copyright 2006-2007 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#include "test_assoc_cont_factory.hpp"

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/flyweight/assoc_container_factory.hpp> 
#include <boost/flyweight/flyweight.hpp>
#include <boost/flyweight/refcounted.hpp>
#include <boost/flyweight/simple_locking.hpp>
#include <boost/flyweight/static_holder.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>
#include "lambda_components.hpp"
#include "test_basic_template.hpp"

using namespace boost::flyweights;

struct reverse_set_specifier
{
  template<typename Entry,typename Value>
  struct apply
  {
    typedef std::set<Entry,std::greater<Value> > type;
  };
};

struct assoc_container_factory_flyweight_specifier1
{
  template<typename T>
  struct apply
  {
    typedef flyweight<
      T,
      assoc_container_factory<reverse_set_specifier>
    > type;
  };
};

struct assoc_container_factory_flyweight_specifier2
{
  template<typename T>
  struct apply
  {
    typedef flyweight<
      T,
      assoc_container_factory_class<
        lambda_set<
          boost::mpl::_1,
          lambda_greater<boost::mpl::_2>

#if !defined(BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)
          ,std::allocator<boost::mpl::_1>
#endif
        >
      >
    > type;
  };
};

void test_assoc_container_factory()
{
  BOOST_FLYWEIGHT_TEST_BASIC(assoc_container_factory_flyweight_specifier1)
  BOOST_FLYWEIGHT_TEST_BASIC(assoc_container_factory_flyweight_specifier2)
}
