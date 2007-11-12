/* Copyright 2006-2007 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_SET_FACTORY_HPP
#define BOOST_FLYWEIGHT_SET_FACTORY_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/detail/allocator_utilities.hpp>
#include <boost/flyweight/assoc_container_factory.hpp>
#include <boost/flyweight/factory_tag.hpp>
#include <boost/flyweight/set_factory_fwd.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>
#include <boost/mpl/if.hpp>
#include <set>

/* Particularization of assoc_container_factory_class using a set.
 */

namespace boost{

namespace flyweights{

template<
  typename Entry,typename Value,
  typename Compare,typename Allocator
>
class set_factory_class:
  public assoc_container_factory_class<
    std::set<
      Entry,
	  typename boost::mpl::if_<
	    mpl::is_na<Compare>,
	    std::less<Value>,
	    Compare
	  >::type,
	  typename boost::mpl::if_<
	    mpl::is_na<Allocator>,
	    std::allocator<Entry>,
	    Allocator
	  >::type
    >
  >
{
public:
  typedef set_factory_class type;
  BOOST_MPL_AUX_LAMBDA_SUPPORT(
    4,set_factory_class,(Entry,Value,Compare,Allocator))
};

/* set_factory_class specifier */

namespace detail{

/* MSVC 6.0 gets choked on MPL apply code if this level of indirection
 * is not used.
 */

template<typename Entry,typename Value,typename Compare,typename Allocator>
struct set_factory_class_fwd
{
  typedef set_factory_class<Entry,Value,Compare,Allocator> type;

  BOOST_MPL_AUX_LAMBDA_SUPPORT(
    4,set_factory_class_fwd,(Entry,Value,Compare,Allocator))
};

} /* namespace flyweights::detail */

template<
  typename Compare,typename Allocator
  BOOST_FLYWEIGHT_NOT_A_PLACEHOLDER_EXPRESSION_DEF
>
struct set_factory:factory_marker
{
  template<typename Entry,typename Value>
  struct apply:
    mpl::apply2<
      detail::set_factory_class_fwd<
        boost::mpl::_1,boost::mpl::_2,Compare,Allocator
      >,
      Entry,Value
    >
  {};
};

} /* namespace flyweights */

} /* namespace boost */

#endif
