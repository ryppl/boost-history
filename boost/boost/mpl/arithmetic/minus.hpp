//-----------------------------------------------------------------------------
// boost mpl/arithmetic/minus.hpp header file
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

#ifndef BOOST_MPL_ARITHMETIC_MINUS_HPP
#define BOOST_MPL_ARITHMETIC_MINUS_HPP

#include "boost/mpl/int_t.hpp"
#include "boost/mpl/et.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

namespace detail {

template< 
      typename T1
    , typename T2
    , typename T3 = mpl::int_t<0>
    , typename T4 = mpl::int_t<0>
    , typename T5 = mpl::int_t<0>
    >
struct minus_impl
{
    BOOST_STATIC_CONSTANT(long, value = (T1::value - minus_impl<T2, T3, T4, T5>::value));
};

template<>
struct minus_impl< 
      mpl::int_t<0>
    , mpl::int_t<0>
    , mpl::int_t<0>
    , mpl::int_t<0>
    , mpl::int_t<0>
    >
{
    BOOST_STATIC_CONSTANT(long, value = 0);
};

} // namespace detail

template< 
      typename T1
    , typename T2
    , typename T3 = mpl::int_t<0>
    , typename T4 = mpl::int_t<0>
    , typename T5 = mpl::int_t<0>
    >
struct minus
{
    BOOST_STATIC_CONSTANT(long
        , value = (::boost::mpl::detail::minus_impl<T1, T2, T3, T4, T5>::value)
        );

    typedef mpl::int_t<
        ::boost::mpl::detail::minus_impl<T1, T2, T3, T4, T5>::value
        > type;
};

// limited forms of ET, just for now
template<>
struct minus< 
      mpl::_1
    , mpl::_2
    , mpl::int_t<0>
    , mpl::int_t<0>
    , mpl::int_t<0>
    >
{
    template<typename U1, typename U2> struct apply
    {
        typedef mpl::int_t<(U1::value - U2::value)> type;
    };
};

template<>
struct minus< 
      mpl::_2
    , mpl::_1
    , mpl::int_t<0>
    , mpl::int_t<0>
    , mpl::int_t<0>
    >
{
    template<typename U1, typename U2> struct apply
    {
        typedef mpl::int_t<(U2::value - U1::value)> type;
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_ARITHMETIC_MINUS_HPP
