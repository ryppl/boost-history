//-----------------------------------------------------------------------------
// boost mpl/compose/f_x_x.hpp header file
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

#ifndef BOOST_MPL_COMPOSE_F_X_X_HPP
#define BOOST_MPL_COMPOSE_F_X_X_HPP

#include "boost/mpl/binary_function.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<
      typename BinaryFunction
    , typename U
    >
struct f_x_x
{
    typedef typename mpl::binary_function<
          BinaryFunction
        , U
        , U
        > type;
};

} // namespace detail

template<typename BinaryFunction>
struct compose_f_x_x
{
    template<typename U> struct apply
        : mpl::detail::f_x_x<
                BinaryFunction
              , U
              >::type
    {
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_COMPOSE_F_X_X_HPP
