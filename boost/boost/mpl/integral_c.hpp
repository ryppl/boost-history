//-----------------------------------------------------------------------------
// boost mpl/intergal_c.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_INTEGRAL_C_HPP_INCLUDED
#define BOOST_MPL_INTEGRAL_C_HPP_INCLUDED

#include "boost/config.hpp"

namespace boost {
namespace mpl {

// for 'integral_ops'
template< typename T, T N > struct integral_c;

namespace aux {
template< typename T >
struct integral_ops
{
    // increment/decrement support
    template<T N> struct next_prior
    {
        // agurt, 15/jan/02: MSVC6.5 workaround; the compiler gives an ICE 
        // if you write 'N + 1' instead of 'value + 1'
        BOOST_STATIC_CONSTANT(T, value = N);
        
        typedef integral_c<T, value + 1> next;
        typedef integral_c<T, value - 1> prior;
    };
};
} // namespace aux

template< typename T, T N >
struct integral_c
    : aux::integral_ops<T>::template next_prior<N>
{
    BOOST_STATIC_CONSTANT(T, value = N);
    typedef integral_c<T, N> type;

    // user-declared default ctor is required for definition of namespace 
    // scope constants
    integral_c() {}

    // enables uniform function call syntax for families of overloaded 
    // functions that return objects of both arithmetic ('int', 'long',
    // 'double', etc.) and 'integral_c<..>' types (for an example, see 
    // "lib/mpl/example/power.cpp")
    operator T() const { return this->value; } 
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_INTEGRAL_C_HPP_INCLUDED
