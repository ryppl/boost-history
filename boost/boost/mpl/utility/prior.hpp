//-----------------------------------------------------------------------------
// boost mpl/utility/prior.hpp header file
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

#ifndef BOOST_MPL_UTILITY_PRIOR_HPP
#define BOOST_MPL_UTILITY_PRIOR_HPP

#include "boost/mpl/et.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<typename T>
struct prior
{
    typedef typename T::prior type;
};

#if defined(BOOST_MSVC)
// workaround of MSVC early template instantiation bug
template<>
struct prior<int>
{
    typedef prior<int> type;
};
#endif

// limited forms of ET, just for now
template<>
struct prior<mpl::_1>
{
    template<typename U> struct apply
    {
        typedef typename prior<U>::type type;
    };
};

template<>
struct prior<mpl::placeholder>
{
    template<typename U> struct apply
    {
        typedef typename prior<U>::type type;
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_UTILITY_PRIOR_HPP
