/* Copyright 2006-2008 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_DETAIL_HAS_STATIC_ENTRY_HPP
#define BOOST_FLYWEIGHT_DETAIL_HAS_STATIC_ENTRY_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/mpl/bool.hpp>

namespace boost{

namespace flyweights{

namespace detail{

/* has_static_entry<Factory> tries to determine whether Factory::entry(h),
 * with h a Factory::handle_type, is a valid expression.
 * Cases where entry is not defined, not a memfun, is overloaded or is a
 * member function template are tolerated, i.e. they do not trigger a
 * compilation error.
 * False negatives may occur, false positives will not.
 */

struct has_static_entry_helper
{
  typedef char&       yes;
  typedef char      (&no)[2];

  template <typename Entry,typename Handle,const Entry& (*)(const Handle&)>
  struct pf1;
  template <typename Entry,typename Handle,const Entry& (*)(Handle)>
  struct pf2;
  
  template<typename Factory,typename Entry,typename Handle>
  static yes check(int,pf1<Entry,Handle,&Factory::entry>* =0);
  template<typename Factory,typename Entry,typename Handle>
  static yes check(char,pf2<Entry,Handle,&Factory::entry>* =0);
  template<typename Factory,typename Entry,typename Handle>
  static no  check(...);
};

template<typename Factory,typename Entry>
struct has_static_entry:
  boost::mpl::bool_<
    sizeof(
      has_static_entry_helper::check<
        Factory,Entry,typename Factory::handle_type
      >(0))==
    sizeof(has_static_entry_helper::yes)
   >
{};

} /* namespace flyweights::detail */

} /* namespace flyweights */

} /* namespace boost */

#endif
