//-----------------------------------------------------------------------------
// boost mpl/math/log10.hpp header file
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

#ifndef BOOST_MPL_MATH_LOG10_HPP
#define BOOST_MPL_MATH_LOG10_HPP

#include "boost/mpl/arithmetic/plus.hpp"
#include "boost/mpl/select_type.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<unsigned long N>
struct log10
{
    BOOST_STATIC_CONSTANT(long, value =
        (mpl::select_type<(N < 10)
            , mpl::int_t<0>
            , mpl::plus< mpl::int_t<1>,log10_t<(N / 10)> >
            >::type::value)
        );

    typedef mpl::int_t<value> type;
};

template<typename N>
struct log10_t : log10<N::value>
{
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_MATH_LOG10_HPP
