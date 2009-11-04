// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_SIDE_HPP
#define GGL_STRATEGIES_SIDE_HPP


#include <ggl/strategies/tags.hpp>


namespace ggl
{


/*!
    \brief Traits class binding a side determination strategy to a coordinate system
    \ingroup util
    \tparam Tag tag of coordinate system of point-type
*/
template <typename Tag>
struct strategy_side
{
    typedef strategy::not_implemented type;
};


} // namespace ggl

#endif // GGL_STRATEGIES_SIDE_HPP
