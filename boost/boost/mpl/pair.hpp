//-----------------------------------------------------------------------------
// boost mpl/pair.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_PAIR_HPP
#define BOOST_MPL_PAIR_HPP

#include "boost/mpl/et.hpp"

namespace boost {
namespace mpl {

template<typename T1, typename T2>
struct pair
{
    typedef T1 first;
    typedef T2 second;
};

template<typename T1, typename T2>
struct make_pair
{
    typedef pair<T1,T2> type;
};

// limited forms of ET, just for now
template<>
struct make_pair< 
      mpl::_1
    , mpl::_2
    >
{
    template<typename U1, typename U2> struct apply
        : make_pair<U1,U2>
    {
    };
};

template<>
struct make_pair< 
      mpl::_2
    , mpl::_1
    >
{
    template<typename U1, typename U2> struct apply
        : make_pair<U2,U1>
    {
    };
};

template<>
struct make_pair< 
      mpl::placeholder
    , mpl::placeholder
    >
{
    template<typename U1, typename U2> struct apply
        : make_pair<U1,U2>
    {
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_PAIR_HPP
