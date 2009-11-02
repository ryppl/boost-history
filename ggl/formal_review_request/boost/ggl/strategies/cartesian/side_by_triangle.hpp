// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGY_CARTESIAN_SIDE_HPP
#define GGL_STRATEGY_CARTESIAN_SIDE_HPP



#include <ggl/geometries/segment.hpp>

#include <ggl/util/select_coordinate_type.hpp>



namespace ggl
{
namespace strategy
{
    namespace side
    {

        template <typename P, typename PS>
        struct xy_side
        {
            typedef typename select_coordinate_type<P, PS>::type coordinate_type;

            // Check at which side of a segment a point lies:
            // left of segment (> 0), right of segment (< 0), on segment (0)
            // In fact this is twice the area of a triangle
            static inline coordinate_type
                side(segment<const PS> const& s, P const& p)
            {
                coordinate_type const x = get<0>(p);
                coordinate_type const y = get<1>(p);

                coordinate_type const sx1 = get<0, 0>(s);
                coordinate_type const sy1 = get<0, 1>(s);
                coordinate_type const sx2 = get<1, 0>(s);
                coordinate_type const sy2 = get<1, 1>(s);

                // Todo: might be changed to subtract_point
                coordinate_type dx = sx2 - sx1;
                coordinate_type dy = sy2 - sy1;
                coordinate_type dpx = x - sx1;
                coordinate_type dpy = y - sy1;
                return dx * dpy - dy * dpx;
            }


            static inline int side(PS const& p0, PS const& p1, P const& p2)
            {
                coordinate_type s = side(segment<const PS>(p0, p1), p2);
                return s > 0 ? 1 : s < 0 ? -1 : 0;
            }
        };

    } // namespace side
} // namespace strategy


#ifndef DOXYGEN_NO_STRATEGY_SPECIALIZATIONS
template <typename P, typename PS>
struct strategy_side<cartesian_tag, P, PS>
{
    typedef strategy::side::xy_side<P, PS> type;
};
#endif

} // namespace ggl


#endif // GGL_STRATEGY_CARTESIAN_SIDE_HPP
