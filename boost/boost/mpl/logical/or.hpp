//-----------------------------------------------------------------------------
// boost mpl/logical/or.hpp header file
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

#ifndef BOOST_MPL_LOGICAL_OR_HPP
#define BOOST_MPL_LOGICAL_OR_HPP

#include "boost/mpl/select_type.hpp"
#include "boost/mpl/bool_t.hpp"
#include "boost/mpl/et.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template< 
      typename T1
    , typename T2
    , typename T3 = mpl::false_t
    , typename T4 = mpl::false_t
    , typename T5 = mpl::false_t
    >
struct logical_or
{
    typedef typename mpl::select_type< 
          T1::value
        , mpl::true_t
        , logical_or<T2, T3, T4, T5>
        >::type type_;

    typedef type_ type;
    BOOST_STATIC_CONSTANT(bool, value = type::value);
};

template<>
struct logical_or< 
      mpl::false_t
    , mpl::false_t
    , mpl::false_t
    , mpl::false_t
    , mpl::false_t
    >
{
    typedef mpl::false_t type;
    BOOST_STATIC_CONSTANT(bool, value = false);
};

// limited forms of ET, just for now
template<>
struct logical_or< 
      mpl::placeholder
    , mpl::placeholder
    , mpl::false_t
    , mpl::false_t
    , mpl::false_t
    >
{
    template<typename U1, typename U2> struct apply
        : mpl::logical_or<U1,U2>
    {
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_LOGICAL_OR_HPP
