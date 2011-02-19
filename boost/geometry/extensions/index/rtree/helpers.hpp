// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.SpatialIndex - geometry helper functions
//
// Copyright 2008 Federico J. Fernandez.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_HELPERS_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_HELPERS_HPP

#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/disjoint.hpp>
#include <boost/geometry/core/point_type.hpp>

// awulkiew - added
#include <boost/geometry/algorithms/combine.hpp>
#include <boost/geometry/algorithms/convert.hpp>

namespace boost { namespace geometry { namespace index {

/**
 * \brief Given two boxes, returns the minimal box that contains them
 */
// TODO: use geometry::combine
// awulkiew - geometry::combine used
//template <typename Box>
//inline Box enlarge_box(Box const& b1, Box const& b2)
//{
//    // TODO: mloskot - Refactor to readable form. Fix VC++8.0 min/max warnings:
//    //  warning C4002: too many actual parameters for macro 'min
//
//    typedef typename geometry::point_type<Box>::type point_type;
//
//    point_type pmin(
//        geometry::get<min_corner, 0>(b1) < geometry::get<min_corner, 0>(b2)
//            ? geometry::get<min_corner, 0>(b1) : geometry::get<min_corner, 0>(b2),
//        geometry::get<min_corner, 1>(b1) < geometry::get<min_corner, 1>(b2)
//            ? geometry::get<min_corner, 1>(b1) : geometry::get<min_corner, 1>(b2));
//
//    point_type pmax(
//        geometry::get<max_corner, 0>(b1) > geometry::get<max_corner, 0>(b2)
//            ? geometry::get<max_corner, 0>(b1) : geometry::get<max_corner, 0>(b2),
//        geometry::get<max_corner, 1>(b1) > geometry::get<max_corner, 1>(b2)
//            ? geometry::get<max_corner, 1>(b1) : geometry::get<max_corner, 1>(b2));
//
//    return Box(pmin, pmax);
//}

/**
 * \brief Compute the area of the union of b1 and b2
 */
template <typename Box, typename Geometry>
inline typename area_result<Box>::type compute_union_area(Box const& b, Geometry const& g)
{
    //Box enlarged_box = enlarge_box(b1, b2);
    // awulkiew - changed to geometry::combine
    Box enlarged_box(b);
    geometry::combine(enlarged_box, g);
    return geometry::area(enlarged_box);
}

/**
 * \brief Checks if boxes intersects
 */
// TODO: move to geometry::intersects
// awulkiew - geometry::intersects used
//template <typename Geometry1, typename Geometry2>
//inline bool is_overlapping(Geometry1 const& b1, Geometry2 const& b2)
//{
//    return ! geometry::disjoint(b1, b2);
//}

// awulkiew - structures and functions added below

namespace dispatch {

template <size_t D, typename SrcBox, typename DstBox>
struct copy_box
{
    BOOST_STATIC_ASSERT(0 < D);

    static void apply(SrcBox const& src, DstBox & dst)
    {
        geometry::set<min_corner, D - 1>(dst, geometry::get<min_corner, D - 1>(src));
        geometry::set<max_corner, D - 1>(dst, geometry::get<max_corner, D - 1>(src));

        copy_box<D - 1, SrcBox, DstBox>::apply(src, dst);
    }
};

template <typename SrcBox, typename DstBox>
struct copy_box<1, SrcBox, DstBox>
{
    static void apply(SrcBox const& src, DstBox & dst)
    {
        geometry::set<min_corner, 0>(dst, geometry::get<min_corner, 0>(src));
        geometry::set<max_corner, 0>(dst, geometry::get<max_corner, 0>(src));
    }
};

template <size_t D, typename Box>
struct copy_box<D, Box, Box>
{
    BOOST_STATIC_ASSERT(0 < D);

    static void apply(Box const& src, Box & dst)
    {
        dst = src;
    }
};

} // namespace dispatch

namespace detail {

template <typename SrcBox, typename DstBox>
void copy_box(SrcBox const& src, DstBox & dst)
{
    BOOST_STATIC_ASSERT(
        traits::dimension<typename traits::point_type<SrcBox>::type>::value
        == traits::dimension<typename traits::point_type<DstBox>::type>::value
    );

    dispatch::copy_box<
        traits::dimension<typename traits::point_type<SrcBox>::type>::value,
        SrcBox,
        DstBox
    >::apply(src, dst);
}

} // namespace detail

namespace dispatch {

template <typename BoundingObject, typename BoundingObjectTag, typename Box>
struct convert_to_box
{
};

template <typename BoundingObject, typename Box>
struct convert_to_box<BoundingObject, geometry::box_tag, Box>
{
    static void apply(BoundingObject const& bo, Box & b)
    {
        detail::copy_box(bo, b);
    }
};

template <typename BoundingObject, typename Box>
struct convert_to_box<BoundingObject, geometry::point_tag, Box>
{
    static void apply(BoundingObject const& bo, Box & b)
    {
        geometry::convert(bo, b);
    }
};

} // namespace dispatch

namespace detail {

template <typename BoundingObject, typename Box>
void convert_to_box(BoundingObject const& bo, Box & b)
{
    dispatch::convert_to_box<
        BoundingObject,
        typename traits::tag<BoundingObject>::type,
        Box
    >::apply(bo, b);
}

} // namespace detail

namespace dispatch {

template <typename BoundingGeometry, typename BoundingGeometryTag>
struct bounding_box
{
    typedef void type;
};

template <typename BoundingGeometry>
struct bounding_box<BoundingGeometry, geometry::box_tag>
{
    typedef BoundingGeometry type;
};

template <typename BoundingGeometry>
struct bounding_box<BoundingGeometry, geometry::point_tag>
{
    typedef geometry::model::box<BoundingGeometry> type;
};

} // namespace dispatch

namespace detail {

template <typename BoundingGeometry>
struct bounding_box
{
    typedef typename dispatch::bounding_box<
        BoundingGeometry,
        typename traits::tag<BoundingGeometry>::type
    >::type type;
};

} // namespace detail

}}} // namespace boost::geometry::index

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_HELPERS_HPP
