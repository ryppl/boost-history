//-----------------------------------------------------------------------------
// boost mpl/identity/select1st.hpp header file
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

#ifndef BOOST_MPL_IDENTITY_SELECT1ST_HPP
#define BOOST_MPL_IDENTITY_SELECT1ST_HPP

#include "boost/mpl/et.hpp"

namespace boost {
namespace mpl {

template<typename Pair>
struct select1st
{
    typedef typename Pair::first type;
};

// limited forms of ET, just for now
template<>
struct select1st<mpl::_1>
{
    template<typename U> struct apply
        : select1st<U>
    {
    };
};

template<>
struct select1st<mpl::placeholder>
{
    template<typename U> struct apply
        : select1st<U>
    {
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_IDENTITY_SELECT1ST_HPP
