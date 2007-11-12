/* Copyright 2006 Joaqu�n M L�pez Mu�oz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_DETAIL_PREVENT_ETI_HPP
#define BOOST_FLYWEIGHT_DETAIL_PREVENT_ETI_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
#include <boost/mpl/if.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/aux_/msvc_never_true.hpp>
#endif

namespace boost{

namespace flyweights{

namespace detail{

#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
/* See
 * http://www.crystalclearsoftware.com/cgi-bin/boost_wiki/wiki.pl?Effective_MPL
 * Item 5.6, Beware of the 'early template instantiation' trap.
 */

template<typename Type,typename Construct>
struct prevent_eti
{
  typedef typename mpl::if_<
    mpl::aux::msvc_never_true<Type>,
    mpl::integral_c<int,0>,
    Construct
  >::type type;
};
#else
template<typename Type,typename Construct>
struct prevent_eti
{
  typedef Construct type;
};
#endif

} /* namespace flyweights::detail */

} /* namespace flyweights */

} /* namespace boost */

#endif
