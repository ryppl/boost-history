//-----------------------------------------------------------------------------
// boost mpl/compose/f_gxy.hpp header file
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

#ifndef BOOST_MPL_COMPOSE_F_GXY_HPP
#define BOOST_MPL_COMPOSE_F_GXY_HPP

#include "boost/mpl/unary_function.hpp"
#include "boost/mpl/binary_function.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<
      typename UnaryFunction
    , typename BinaryFunction
    , typename U1
    , typename U2
    >
struct f_gxy
{
    typedef typename mpl::binary_function<BinaryFunction,U1,U2>::type P;
    typedef typename mpl::unary_function<
          UnaryFunction
        , P
        > type;
};

} // namespace detail

template<
      typename UnaryFunction
    , typename BinaryFunction
    >
struct compose_f_gxy
{
    template<typename U1, typename U2> struct apply
        : mpl::detail::f_gxy<
                UnaryFunction
              , BinaryFunction
              , U1
              , U2
              >::type
    {
    };
};

template<
      typename UnaryFunction
    , typename BinaryFunction
    >
struct compose_f_gyx
{
    template<typename U1, typename U2> struct apply
        : mpl::detail::f_gxy<
                UnaryFunction
              , BinaryFunction
              , U2
              , U1
              >::type
    {
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_COMPOSE_F_GXY_HPP
