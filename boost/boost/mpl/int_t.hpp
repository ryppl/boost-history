//-----------------------------------------------------------------------------
// boost mpl/int_t.hpp header file
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

#ifndef BOOST_MPL_INT_T_HPP
#define BOOST_MPL_INT_T_HPP

#include "boost/config.hpp"
#include <climits>

namespace boost {
namespace mpl {

template<long N>
struct int_t
{
    BOOST_STATIC_CONSTANT(long, value = N);
    typedef int_t<N> type;

    // increment/decrement support
    typedef int_t<N + 1> next;
    typedef int_t<N - 1> prior;

    // user-declared default ctor is required for definition of namespace 
    // scope constants _0, _1, etc.
    int_t() {}

    // enables uniform function call syntax for families of overloaded 
    // functions that return objects of both arithmetic ('int', 'long',
    // 'double', etc.) and 'int_t<..>' types (for an example, see 
    // "lib/mpl/example/power.cpp")
    operator int() const { return this->value; } 
};

template<>
struct int_t<LONG_MAX>
{
    BOOST_STATIC_CONSTANT(long, value = LONG_MAX);
    typedef int_t<LONG_MAX> type;
    typedef int_t<LONG_MAX - 1> prior;

    int_t() {}
    operator int() const { return this->value; }
};

template<>
struct int_t<LONG_MIN>
{
    BOOST_STATIC_CONSTANT(long, value = LONG_MIN);
    typedef int_t<LONG_MIN> type;
    typedef int_t<LONG_MIN + 1> next;

    int_t() {}
    operator int() const { return this->value; }
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_INT_T_HPP
