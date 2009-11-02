// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_POINT_IN_POLY_HPP
#define GGL_STRATEGIES_POINT_IN_POLY_HPP

#include <ggl/strategies/tags.hpp>

namespace ggl
{


/*!
    \brief Traits class binding a within determination strategy to a coordinate system
    \ingroup within
    \tparam TagPoint tag of coordinate system of point-type
    \tparam TagSegment tag of coordinate system of segment-type
    \tparam Point point-type of input points
    \tparam PointOfSegment point-type of input segment-points
*/
template <typename TagPoint, typename TagSegment, typename Point, typename PointOfSegment>
struct strategy_within
{
    typedef strategy::not_implemented type;
};



} // namespace ggl

#endif // GGL_STRATEGIES_POINT_IN_POLY_HPP
