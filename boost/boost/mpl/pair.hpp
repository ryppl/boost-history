
#ifndef BOOST_MPL_PAIR_HPP_INCLUDED
#define BOOST_MPL_PAIR_HPP_INCLUDED

// + file: boost/mpl/pair.hpp
// + last modified: 10/jun/03

// Copyright (c) 2001-03
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.
//
// See http://www.boost.org/libs/mpl for documentation.

#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/aux_/lambda_support.hpp"
#include "boost/mpl/aux_/config/eti.hpp"

namespace boost {
namespace mpl {

template<
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(T1)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(T2)
    >
struct pair
{
    typedef pair type;
    typedef T1 first;
    typedef T2 second;

    BOOST_MPL_AUX_LAMBDA_SUPPORT(2, pair, (T1,T2))
};


template<
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Pair)
    >
struct first
{
    typedef typename Pair::first type;
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1, first, (Pair))
};


template<
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Pair)
    >
struct second
{
    typedef typename Pair::second type;
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1, second, (Pair))
};


#if defined(BOOST_MPL_MSVC_60_ETI_BUG)
template<> struct first<int> { typedef int type; };
template<> struct second<int> { typedef int type; };
#endif

BOOST_MPL_AUX_VOID_SPEC(2, pair)
BOOST_MPL_AUX_VOID_SPEC(1, first)
BOOST_MPL_AUX_VOID_SPEC(1, second)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_PAIR_HPP_INCLUDED
