// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_CONVEX_HULL_HPP
#define GGL_STRATEGIES_CONVEX_HULL_HPP

#include <ggl/strategies/tags.hpp>


namespace ggl
{




/*!
    \brief Traits class binding a convex hull calculation strategy to a coordinate system
    \ingroup convex_hull
    \tparam Tag tag of coordinate system
    \tparam Geometry the geometry type (hull operates internally per hull over geometry)
    \tparam Point point-type of output points
*/
template <typename Tag, typename Geometry, typename Point>
struct strategy_convex_hull
{
    typedef strategy::not_implemented type;
};


} // namespace ggl

#endif // GGL_STRATEGIES_CONVEX_HULL_HPP
