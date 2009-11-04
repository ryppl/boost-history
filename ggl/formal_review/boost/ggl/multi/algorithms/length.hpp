// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_MULTI_ALGORITHMS_LENGTH_HPP
#define GGL_MULTI_ALGORITHMS_LENGTH_HPP

#include <ggl/algorithms/length.hpp>
#include <ggl/multi/core/tags.hpp>
#include <ggl/multi/algorithms/detail/multi_sum.hpp>

namespace ggl
{

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename MultiLinestring, typename Strategy>
struct length<multi_linestring_tag, MultiLinestring, Strategy>
    : detail::multi_sum
        <
            typename length_result<MultiLinestring>::type,
            MultiLinestring,
            Strategy,
            detail::length::range_length
                <
                    typename boost::range_value<MultiLinestring>::type,
                    Strategy
                >
        >
{};


} // namespace dispatch
#endif


} // namespace ggl


#endif // GGL_MULTI_ALGORITHMS_LENGTH_HPP
