//-----------------------------------------------------------------------------
// boost mpl/comparison/equal_to.hpp header file
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

#ifndef BOOST_MPL_COMPARISON_EQUAL_TO_HPP
#define BOOST_MPL_COMPARISON_EQUAL_TO_HPP

#include "boost/mpl/bool_t.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/mpl/et.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template< 
      typename T1
    , typename T2
    >
struct equal_to
{
    BOOST_STATIC_CONSTANT(bool, value = (T1::value == T2::value));
    typedef mpl::bool_t<value> type;
};

// shortcut form for 'equal to N'
template<long N>
struct eq
{
    template<typename U> struct apply
        : equal_to< U,mpl::int_t<N> >
    {
    };
};

// limited forms of ET, just for now
template<>
struct equal_to<
      mpl::_1
    , mpl::_2
    >
{
    template<typename U1, typename U2> struct apply
    {
        typedef typename equal_to<U1,U2>::type type;
    };
};

template<>
struct equal_to< 
      mpl::_2
    , mpl::_1
    >
{
    template<typename U1, typename U2> struct apply
    {
        typedef typename equal_to<U2,U1>::type type;
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_COMPARISON_EQUAL_TO_HPP
