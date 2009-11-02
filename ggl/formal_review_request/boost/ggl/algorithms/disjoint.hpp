// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_DISJOINT_HPP
#define GGL_ALGORITHMS_DISJOINT_HPP


/*!
\defgroup disjoint disjoint, not any spatial relation
\details disjoint means spatially disjoint, there is no overlap of interiors
    and boundaries, the intersection of interiors or boundaries is empty.
*/


#include <boost/mpl/if.hpp>
#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>
#include <boost/static_assert.hpp>

#include <ggl/core/access.hpp>
#include <ggl/core/coordinate_dimension.hpp>
#include <ggl/core/is_multi.hpp>
#include <ggl/core/reverse_dispatch.hpp>

#include <ggl/geometries/concepts/check.hpp>

#include <ggl/util/math.hpp>
#include <ggl/util/select_coordinate_type.hpp>



namespace ggl
{


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace disjoint {


template
<
    typename Point1, typename Point2,
    std::size_t Dimension, std::size_t DimensionCount
>
struct point_point
{
    static inline bool apply(Point1 const& p1, Point2 const& p2)
    {
        if (! ggl::math::equals(get<Dimension>(p1), get<Dimension>(p2)))
        {
            return true;
        }
        return point_point
            <
                Point1, Point2,
                Dimension + 1, DimensionCount
            >::apply(p1, p2);
    }
};


template <typename Point1, typename Point2, std::size_t DimensionCount>
struct point_point<Point1, Point2, DimensionCount, DimensionCount>
{
    static inline bool apply(Point1 const& , Point2 const& )
    {
        return false;
    }
};


template
<
    typename Point, typename Box,
    std::size_t Dimension, std::size_t DimensionCount
>
struct point_box
{
    static inline bool apply(Point const& point, Box const& box)
    {
        if (get<Dimension>(point) < get<min_corner, Dimension>(box)
            || get<Dimension>(point) > get<max_corner, Dimension>(box))
        {
            return true;
        }
        return point_box
            <
                Point, Box,
                Dimension + 1, DimensionCount
            >::apply(point, box);
    }
};


template <typename Point, typename Box, std::size_t DimensionCount>
struct point_box<Point, Box, DimensionCount, DimensionCount>
{
    static inline bool apply(Point const& , Box const& )
    {
        return false;
    }
};


template
<
    typename Box1, typename Box2,
    std::size_t Dimension, std::size_t DimensionCount
>
struct box_box
{
    static inline bool apply(Box1 const& box1, Box2 const& box2)
    {
        if (get<max_corner, Dimension>(box1) < get<min_corner, Dimension>(box2))
        {
            return true;
        }
        if (get<min_corner, Dimension>(box1) > get<max_corner, Dimension>(box2))
        {
            return true;
        }
        return box_box
            <
                Box1, Box2,
                Dimension + 1, DimensionCount
            >::apply(box1, box2);
    }
};


template <typename Box1, typename Box2, std::size_t DimensionCount>
struct box_box<Box1, Box2, DimensionCount, DimensionCount>
{
    static inline bool apply(Box1 const& , Box2 const& )
    {
        return false;
    }
};


}} // namespace detail::disjoint
#endif // DOXYGEN_NO_DETAIL


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{


template
<
    typename GeometryTag1, typename GeometryTag2,
    typename G1, typename G2,
    bool IsMulti1, bool IsMulti2,
    std::size_t DimensionCount
>
struct disjoint
{};


template <typename P1, typename P2, std::size_t DimensionCount>
struct disjoint<point_tag, point_tag, P1, P2, false, false, DimensionCount>
    : detail::disjoint::point_point<P1, P2, 0, DimensionCount>
{};


template <typename B1, typename B2, std::size_t DimensionCount>
struct disjoint<box_tag, box_tag, B1, B2, false, false, DimensionCount>
    : detail::disjoint::box_box<B1, B2, 0, DimensionCount>
{};


template <typename P, typename B, std::size_t DimensionCount>
struct disjoint<point_tag, box_tag, P, B, false, false, DimensionCount>
    : detail::disjoint::point_box<P, B, 0, DimensionCount>
{};


template
<
    typename GeometryTag1, typename GeometryTag2,
    typename G1, typename G2,
    bool IsMulti1, bool IsMulti2,
    std::size_t DimensionCount
>
struct disjoint_reversed
{
    static inline bool apply(G1 const& g1, G2 const& g2)
    {
        return disjoint
            <
                GeometryTag2, GeometryTag1,
                G2, G1,
                IsMulti2, IsMulti1,
                DimensionCount
            >::apply(g2, g1);
    }
};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH



/*!
    \brief Calculate if two geometries are disjoint
    \ingroup disjoint
    \tparam Geometry1 first geometry type
    \tparam Geometry2 second geometry type
    \param geometry1 first geometry
    \param geometry2 second geometry
    \return true if disjoint, else false
 */
template <typename Geometry1, typename Geometry2>
inline bool disjoint(const Geometry1& geometry1,
            const Geometry2& geometry2)
{
    concept::check_concepts_and_equal_dimensions
        <
            const Geometry1,
            const Geometry2
        >();

    typedef typename boost::remove_const<Geometry1>::type ncg1_type;
    typedef typename boost::remove_const<Geometry2>::type ncg2_type;

    return boost::mpl::if_c
        <
            reverse_dispatch<Geometry1, Geometry2>::type::value,
            dispatch::disjoint_reversed
            <
                typename tag<ncg1_type>::type,
                typename tag<ncg2_type>::type,
                ncg1_type,
                ncg2_type,
                is_multi<ncg1_type>::type::value,
                is_multi<ncg2_type>::type::value,
                dimension<ncg1_type>::type::value
            >,
            dispatch::disjoint
            <
                typename tag<ncg1_type>::type,
                typename tag<ncg2_type>::type,
                ncg1_type,
                ncg2_type,
                is_multi<ncg1_type>::type::value,
                is_multi<ncg2_type>::type::value,
                dimension<ncg1_type>::type::value
            >
        >::type::apply(geometry1, geometry2);
}


} // namespace ggl

#endif // GGL_ALGORITHMS_DISJOINT_HPP
