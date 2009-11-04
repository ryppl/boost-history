// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_EQUALS_HPP
#define GGL_ALGORITHMS_EQUALS_HPP

/*!
\defgroup equals equals: detect if two geometries are spatially equal
\par Geometries:
- \b point + \b point
- \b box + \b box

*/


#include <cstddef>
#include <deque>

#include <boost/static_assert.hpp>
#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>

#include <ggl/core/access.hpp>
#include <ggl/core/coordinate_dimension.hpp>
#include <ggl/core/is_multi.hpp>
#include <ggl/core/interior_rings.hpp>

#include <ggl/algorithms/detail/disjoint.hpp>
#include <ggl/algorithms/detail/not.hpp>
#include <ggl/algorithms/area.hpp>
#include <ggl/algorithms/overlay/get_intersection_points.hpp>
#include <ggl/algorithms/overlay/merge_intersection_points.hpp>
#include <ggl/geometries/concepts/check.hpp>
#include <ggl/util/math.hpp>



namespace ggl
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace equals {


template
<
    typename Box1,
    typename Box2,
    std::size_t Dimension,
    std::size_t DimensionCount
>
struct box_box
{
    static inline bool apply(Box1 const& box1, Box2 const& box2)
    {
        if (!ggl::math::equals(get<min_corner, Dimension>(box1), get<min_corner, Dimension>(box2))
            || !ggl::math::equals(get<max_corner, Dimension>(box1), get<max_corner, Dimension>(box2)))
        {
            return false;
        }
        return box_box<Box1, Box2, Dimension + 1, DimensionCount>::apply(box1, box2);
    }
};

template <typename Box1, typename Box2, std::size_t DimensionCount>
struct box_box<Box1, Box2, DimensionCount, DimensionCount>
{
    static inline bool apply(Box1 const& , Box2 const& )
    {
        return true;
    }
};


template <typename Ring1, typename Ring2>
struct ring_ring
{
    static inline bool apply(Ring1 const& ring1, Ring2 const& ring2, bool check_area = true)
    {
        // Note: this implementation makes use of getting interections
        // and merge them. If all IP's disappear, the ring should be the same
        // (because merging removes collinear or non-collinear
        // IP's following the same path)
        // However, this implementation should be redone using
        // a linear time algorithm (getting left-most points of both
        // and follow them using circular iterator and distance/side)

        // obvious check, area's should be the same.
        if (check_area && ggl::area(ring1) != ggl::area(ring2))
        {
            return false;
        }
        // We could check more (perimeter,centroid,envelope)
        // For now we go directly to intersection points

        typedef typename ggl::point_type<Ring1>::type point_type;
        typedef detail::intersection::intersection_point<point_type> ip_type;
        typedef std::deque<ip_type> container_type;

        container_type ips;


        bool trivial = ggl::get_intersection_points(ring1, ring2, ips);
        if (trivial || ips.size() == 0)
        {
            return false;
        }
        ggl::merge_intersection_points(ips);
        if (ips.size() == 0)
        {
            // All IP's are merged. Meaning collinear / path can be followed
            return true;
        }

        return false;
    }
};


template <typename Polygon1, typename Polygon2>
struct polygon_polygon
{

    struct sortable
    {
        int index;
        double area;
        bool counterpart_found;
        inline sortable(int i, double a)
            : index(i)
            , area(a)
            , counterpart_found(false)
        {}

        inline bool operator<(sortable const& other) const
        {
            return area < other.area;
        }
    };


    template <typename Poly>
    static inline void fill_sortable(Poly const& polygon, std::vector<sortable>& v)
    {
        int i = 0;
        for (typename boost::range_const_iterator
                <
                    typename interior_type<Poly>::type
                >::type it = boost::begin(interior_rings(polygon));
             it != boost::end(interior_rings(polygon));
             ++it, ++i)
        {
            v.push_back(sortable(i, ggl::area(*it)));
        }
        std::sort(v.begin(), v.end());

        /***
        for (typename std::vector<sortable>::iterator it = v.begin();
            it != v.end();
            ++it)
        {
            std::cout << "Ring: " << it->index << " area: " << it->area << std::endl;
        }
        ***/
    }

    static inline bool apply(Polygon1 const& polygon1, Polygon2 const& polygon2)
    {
        // Check number of rings (area check is done in exterior ring check)
        if (ggl::num_interior_rings(polygon1) != ggl::num_interior_rings(polygon2))
        {
            return false;
        }

        typedef typename ggl::ring_type<Polygon1>::type ring_type1;
        typedef typename ggl::ring_type<Polygon2>::type ring_type2;
        typedef ring_ring<ring_type1, ring_type2> compare;

        // Check exterior ring
        if (! compare::apply(exterior_ring(polygon1), exterior_ring(polygon2)))
        {
            return false;
        }

        // Check interior rings -> first sort them on area ,
        // for performance reasons (area is not re-calculated in ring-compare)
        std::vector<sortable> int1, int2;

        fill_sortable(polygon1, int1);
        fill_sortable(polygon2, int2);

        std::size_t n = 0;

        typename interior_type<Polygon1>::type const& rings1
                = interior_rings(polygon1);
        typename interior_type<Polygon2>::type const& rings2
                = interior_rings(polygon2);

        // Compare all rings (having equal area)
        for (typename std::vector<sortable>::iterator it1 = int1.begin();
            it1 != int1.end();
            ++it1)
        {
            for (typename std::vector<sortable>::iterator it2 = int2.begin();
                ! it1->counterpart_found
                    && it2 != int2.end()
                    && it2->area <= it1->area; //+epsilon
                ++it2)
            {
                if (! it2->counterpart_found
                    && ggl::math::equals(it1->area, it2->area))
                {
                    if (compare::apply(rings1[it1->index],
                            rings2[it2->index], false))
                    {
                        it1->counterpart_found = true;
                        it2->counterpart_found = true;
                        n++;
                    }
                }
            }
        }

        return n == ggl::num_interior_rings(polygon1);
    }
};


}} // namespace detail::equals
#endif // DOXYGEN_NO_DETAIL


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template
<
    typename Tag1, typename Tag2,
    bool IsMulti1, bool IsMulti2,
    typename Geometry1,
    typename Geometry2,
    std::size_t DimensionCount
>
struct equals
{};


template <typename P1, typename P2, std::size_t DimensionCount>
struct equals<point_tag, point_tag, false, false, P1, P2, DimensionCount>
    : ggl::detail::not_
        <
            P1,
            P2,
            detail::disjoint::point_point<P1, P2, 0, DimensionCount>
        >
{};


template <typename Box1, typename Box2, std::size_t DimensionCount>
struct equals<box_tag, box_tag, false, false, Box1, Box2, DimensionCount>
    : detail::equals::box_box<Box1, Box2, 0, DimensionCount>
{};


template <typename Ring1, typename Ring2>
struct equals<ring_tag, ring_tag, false, false, Ring1, Ring2, 2>
    : detail::equals::ring_ring<Ring1, Ring2>
{};

template <typename Polygon1, typename Polygon2>
struct equals<polygon_tag, polygon_tag, false, false, Polygon1, Polygon2, 2>
    : detail::equals::polygon_polygon<Polygon1, Polygon2>
{};

} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


/*!
    \brief Detect if two geometries are spatially equal
    \ingroup equals
    \tparam Geometry1 first geometry type
    \tparam Geometry2 second geometry type
    \param geometry1 first geometry
    \param geometry2 second geometry
    \return true if geometries are spatially equal, else false
 */
template <typename Geometry1, typename Geometry2>
inline bool equals(Geometry1 const& geometry1, Geometry2 const& geometry2)
{
    concept::check_concepts_and_equal_dimensions
        <
            const Geometry1,
            const Geometry2
        >();


    return dispatch::equals
            <
                typename tag<Geometry1>::type,
                typename tag<Geometry2>::type,
                is_multi<Geometry1>::type::value,
                is_multi<Geometry2>::type::value,
                Geometry1,
                Geometry2,
                dimension<Geometry1>::type::value
            >::apply(geometry1, geometry2);
}

} // namespace ggl

#endif // GGL_ALGORITHMS_EQUALS_HPP

