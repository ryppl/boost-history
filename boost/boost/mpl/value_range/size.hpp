//-----------------------------------------------------------------------------
// boost mpl/value_range/size.hpp header file
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

#ifndef BOOST_MPL_VALUE_RANGE_SIZE_HPP
#define BOOST_MPL_VALUE_RANGE_SIZE_HPP

#include "boost/mpl/size.hpp"
#include "boost/mpl/value_range/tags.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<>
struct size_algorithm_traits<mpl::half_open_range_tag>
{
    template<typename Range> struct algorithm
    {
        BOOST_STATIC_CONSTANT(long, value = Range::finish - Range::start);
    };
};

template<>
struct size_algorithm_traits<mpl::closed_range_tag>
{
    template<typename Range> struct algorithm
    {
        BOOST_STATIC_CONSTANT(long, value = Range::finish - Range::start + 1);
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_VALUE_RANGE_SIZE_HPP
