//-----------------------------------------------------------------------------
// boost mpl/compose/f_gx_hx.hpp header file
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

#ifndef BOOST_MPL_COMPOSE_F_GX_HX_HPP
#define BOOST_MPL_COMPOSE_F_GX_HX_HPP

#include "boost/mpl/unary_function.hpp"
#include "boost/mpl/binary_function.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<
      typename BinaryFunction
    , typename UnaryFunction1
    , typename UnaryFunction2
    , typename U
    >
struct f_gx_hx
{
    typedef typename mpl::unary_function<UnaryFunction1,U>::type P1;
    typedef typename mpl::unary_function<UnaryFunction2,U>::type P2;
    typedef typename mpl::binary_function<
          BinaryFunction
        , P1
        , P2
        > type;
};

} // namespace detail

template<
      typename BinaryFunction
    , typename UnaryFunction1
    , typename UnaryFunction2
    >
struct compose_f_gx_hx
{
    template<typename U> struct apply
        : mpl::detail::f_gx_hx<
                BinaryFunction
              , UnaryFunction1
              , UnaryFunction2
              , U
              >::type
    {
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_COMPOSE_F_GX_HX_HPP
