// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_AREA_HPP
#define GGL_STRATEGIES_AREA_HPP

#include <ggl/strategies/tags.hpp>


namespace ggl
{


/*!
    \brief Traits class binding an area strategy to a coordinate system
    \ingroup area
    \tparam Tag tag of coordinate system
    \tparam PointOfSegment point-type
*/
template <typename Tag, typename PointOfSegment>
struct strategy_area
{
    typedef strategy::not_implemented type;
};


} // namespace ggl

#endif // GGL_STRATEGIES_AREA_HPP
