
#ifndef BOOST_MPL_AUX_VALUE_WKND_HPP_INCLUDED
#define BOOST_MPL_AUX_VALUE_WKND_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/aux_/adl_barrier.hpp>
#include <boost/mpl/aux_/config/eti.hpp>
#include <boost/mpl/aux_/config/workaround.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, < 0x600) \
    || defined(BOOST_MPL_CFG_MSVC_60_ETI_BUG)
 
#   include <boost/mpl/int.hpp>

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN
namespace aux {
template< typename C_ > struct value_wknd
    : C_
{
};

#if defined(BOOST_MPL_CFG_MSVC_60_ETI_BUG)
template<> struct value_wknd<int>
    : int_<1>
{
};
#endif
} // namespace aux
BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

#if !defined(BOOST_MPL_CFG_MSVC_60_ETI_BUG)
#   define BOOST_MPL_AUX_VALUE_WKND(C) \
    ::BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::aux::value_wknd< C > \
/**/
#    define BOOST_MPL_AUX_MSVC_VALUE_WKND(C) BOOST_MPL_AUX_VALUE_WKND(C)
#else
#   define BOOST_MPL_AUX_VALUE_WKND(C) C
#   define BOOST_MPL_AUX_MSVC_VALUE_WKND(C) \
    ::BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::aux::value_wknd< C > \
/**/
#endif

#else // !__BORLANDC__

#   define BOOST_MPL_AUX_VALUE_WKND(C) C
#   define BOOST_MPL_AUX_MSVC_VALUE_WKND(C) C

#endif

#endif // BOOST_MPL_AUX_VALUE_WKND_HPP_INCLUDED
