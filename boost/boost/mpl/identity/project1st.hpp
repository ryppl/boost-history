//-----------------------------------------------------------------------------
// boost mpl/identity/project1st.hpp header file
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

#ifndef BOOST_MPL_IDENTITY_PROJECT1ST_HPP
#define BOOST_MPL_IDENTITY_PROJECT1ST_HPP

#include "boost/mpl/et.hpp"

namespace boost {
namespace mpl {

template<typename T1, typename T2>
struct project1st
{
    typedef T1 type;
};

// limited forms of ET, just for now
template<>
struct project1st< 
      mpl::_1
    , mpl::_2
    >
{
    template<typename U1, typename U2> struct apply
    {
        typedef typename project1st<U1,U2>::type type;
    };
};

template<>
struct project1st< 
      mpl::_2
    , mpl::_1
    >
{
    template<typename U1, typename U2> struct apply
    {
        typedef typename project1st<U2,U1>::type type;
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_IDENTITY_PROJECT1ST_HPP
