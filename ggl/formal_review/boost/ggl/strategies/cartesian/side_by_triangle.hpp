// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_CARTESIAN_SIDE_BY_TRIANGLE_HPP
#define GGL_STRATEGIES_CARTESIAN_SIDE_BY_TRIANGLE_HPP



#include <ggl/geometries/segment.hpp>

#include <ggl/util/select_coordinate_type.hpp>



namespace ggl
{
namespace strategy
{
    namespace side
    {

        // Check at which side of a segment a point lies:
        // left of segment (> 0), right of segment (< 0), on segment (0)
        // In fact this is twice the area of a triangle
        struct side_by_triangle
        {

            // Template member functions, because it is not always trivial
            // or convenient to explicitly mention the typenames in the
            // strategy-struct itself.

            // Types can be all three different.

            template <typename P1, typename P2, typename P>
            static inline int apply(P1 const& p1, P2 const& p2, P const& p)
            {
                typedef typename select_most_precise
                    <
                        typename select_most_precise
                            <
                                typename coordinate_type<P1>::type,
                                typename coordinate_type<P2>::type
                            >::type,
                        typename coordinate_type<P>::type
                    >::type coordinate_type;

                coordinate_type const x = get<0>(p);
                coordinate_type const y = get<1>(p);

                coordinate_type const sx1 = get<0>(p1);
                coordinate_type const sy1 = get<1>(p1);
                coordinate_type const sx2 = get<0>(p2);
                coordinate_type const sy2 = get<1>(p2);

                coordinate_type const dx = sx2 - sx1;
                coordinate_type const dy = sy2 - sy1;
                coordinate_type const dpx = x - sx1;
                coordinate_type const dpy = y - sy1;

                coordinate_type s = dx * dpy - dy * dpx;

                return s > 0 ? 1 : s < 0 ? -1 : 0;
            }
        };

    } // namespace side
} // namespace strategy


#ifndef DOXYGEN_NO_STRATEGY_SPECIALIZATIONS
template <>
struct strategy_side<cartesian_tag>
{
    typedef strategy::side::side_by_triangle type;
};
#endif

} // namespace ggl


#endif // GGL_STRATEGIES_CARTESIAN_SIDE_BY_TRIANGLE_HPP
