// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_TRANSFORM_HPP
#define GGL_STRATEGIES_TRANSFORM_HPP

#include <cstddef>

#include <ggl/strategies/tags.hpp>


namespace ggl
{


/*!
    \brief Traits class binding a transformation strategy to a coordinate system
    \ingroup transform
    \details Can be specialized
    - per coordinate system family (tag)
    - per coordinate system (or groups of them)
    - per dimension
    - per point type
    \tparam CoordinateSystemTag 1,2 coordinate system tags
    \tparam CoordinateSystem 1,2 coordinate system
    \tparam D 1, 2 dimension
    \tparam Point 1, 2 point type
 */
template
<
    typename CoordinateSystemTag1, typename CoordinateSystemTag2,
    typename CoordinateSystem1, typename CoordinateSystem2,
    std::size_t Dimension1, std::size_t Dimension2,
    typename Point1, typename Point2
>
struct strategy_transform
{
    typedef strategy::not_implemented type;
};



} // namespace ggl

#endif // GGL_STRATEGIES_TRANSFORM_HPP
