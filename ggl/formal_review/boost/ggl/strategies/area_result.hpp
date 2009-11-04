// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_AREA_RESULT_HPP
#define GGL_STRATEGIES_AREA_RESULT_HPP


#include <ggl/core/cs.hpp>
#include <ggl/core/coordinate_type.hpp>
#include <ggl/strategies/area.hpp>
#include <ggl/util/select_most_precise.hpp>


namespace ggl
{

/*!
    \brief Meta-function defining return type of area function
    \ingroup area
    \note The strategy defines the return-type (so this situation is different
        from length, where distance is sqr/sqrt, but length always squared)

 */
template <typename Geometry>
struct area_result
{
    typedef typename point_type<Geometry>::type point_type;
    typedef typename strategy_area
        <
            typename cs_tag<point_type>::type,
            point_type
        >::type strategy_type;

    typedef typename strategy_type::return_type type;
};


} // namespace ggl


#endif // GGL_STRATEGIES_AREA_RESULT_HPP
