/* Copyright 2006-2007 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_HASHED_FACTORY_HPP
#define BOOST_FLYWEIGHT_HASHED_FACTORY_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/flyweight/factory_tag.hpp>
#include <boost/flyweight/hashed_factory_fwd.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>
#include <boost/mpl/if.hpp>

/* Flyweight factory based on a hashed container implemented
 * with Boost.MultiIndex.
 */

namespace boost{

namespace flyweights{

template<
  typename Entry,typename Value,
  typename Hash,typename Pred,typename Allocator
>
class hashed_factory_class:public factory_marker
{
  struct index_list:
    boost::mpl::vector1<
      multi_index::hashed_unique<
        multi_index::identity<Entry>,
        typename boost::mpl::if_<
          mpl::is_na<Hash>,
	      hash<Value>,
	      Hash
        >::type,
        typename boost::mpl::if_<
	      mpl::is_na<Pred>,
	      std::equal_to<Value>,
	      Pred
        >::type
      >
    >
  {};

  typedef multi_index::multi_index_container<
    Entry,
    index_list,
	typename boost::mpl::if_<
	  mpl::is_na<Allocator>,
	  std::allocator<Entry>,
	  Allocator
	>::type
  > container_type;

public:
  typedef const Entry* handle_type;
  
  handle_type insert(const Entry& x)
  {
  	return &*cont.insert(x).first;
  }

  void erase(handle_type h)
  {
    cont.erase(cont.iterator_to(*h));
  }

  const Entry& entry(handle_type h){return *h;}

private:  
  container_type cont;

public:
  typedef hashed_factory_class type;
  BOOST_MPL_AUX_LAMBDA_SUPPORT(
    5,hashed_factory_class,(Entry,Value,Hash,Pred,Allocator))
};

/* hashed_factory_class specifier */

namespace detail{

/* MSVC 6.0 gets choked on MPL apply code if this level of indirection
 * is not used.
 */

template<
  typename Entry,typename Value,typename Hash,typename Pred,typename Allocator
>
struct hashed_factory_class_fwd
{
  typedef hashed_factory_class<Entry,Value,Hash,Pred,Allocator> type;

  BOOST_MPL_AUX_LAMBDA_SUPPORT(
    5,hashed_factory_class_fwd,(Entry,Value,Hash,Pred,Allocator))
};

} /* namespace flyweights::detail */

template<
  typename Hash,typename Pred,typename Allocator
  BOOST_FLYWEIGHT_NOT_A_PLACEHOLDER_EXPRESSION_DEF
>
struct hashed_factory:factory_marker
{
  template<typename Entry,typename Value>
  struct apply:
    mpl::apply2<
      detail::hashed_factory_class_fwd<
        boost::mpl::_1,boost::mpl::_2,Hash,Pred,Allocator
      >,
      Entry,Value
    >
  {};
};

} /* namespace flyweights */

} /* namespace boost */

#endif
