/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_PREVENT_ETI_HPP
#define BOOST_INDEXED_SET_PREVENT_ETI_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
#include <boost/mpl/if.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/aux_/msvc_never_true.hpp>
#endif

namespace boost{

namespace indexed_sets{

namespace detail{

#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
/* See http://www.crystalclearsoftware.com/cgi-bin/boost_wiki/wiki.pl?Effective_MPL
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

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
