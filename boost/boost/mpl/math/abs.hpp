//-----------------------------------------------------------------------------
// boost mpl/math/abs.hpp header file
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

#ifndef BOOST_MPL_MATH_ABS_HPP
#define BOOST_MPL_MATH_ABS_HPP

#include "boost/mpl/int_t.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<long N>
struct abs
{
    BOOST_STATIC_CONSTANT(unsigned long, value = (N < 0 ? -N : N));
    typedef mpl::int_t<value> type;
};

template<typename N>
struct abs_t
    : abs<N::value>
{
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_MATH_ABS_HPP
