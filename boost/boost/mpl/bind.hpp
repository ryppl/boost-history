//-----------------------------------------------------------------------------
// boost mpl/bind.hpp header file
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

#ifndef BOOST_MPL_BIND_HPP
#define BOOST_MPL_BIND_HPP

#include "boost/mpl/binary_function.hpp"

namespace boost {
namespace mpl {

template<typename BinaryFunction, typename T>
struct bind1st
{
    template<typename U> struct apply
        : mpl::binary_function<BinaryFunction,T,U>
    {
    };
};

template<typename BinaryFunction, typename T>
struct bind2nd
{
    template<typename U> struct apply
        : mpl::binary_function<BinaryFunction,U,T>
    {
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_BIND_HPP
