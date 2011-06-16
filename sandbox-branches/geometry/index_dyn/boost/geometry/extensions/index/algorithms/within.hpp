// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.SpatialIndex - n-dimensional within box
//
// Copyright 2011 Adam Wulkiewicz.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_ALGORITHMS_WITHIN_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_ALGORITHMS_WITHIN_HPP

namespace boost { namespace geometry { namespace index {

namespace dispatch {

template <size_t Corner, size_t DimensionIndex, typename Indexable, typename IndexableTag>
struct within_compare
{
    // TODO: awulkiew - static assert?
};

template <size_t DimensionIndex, typename Indexable>
struct within_compare<min_corner, DimensionIndex, Indexable, box_tag>
{
    template <typename Box>
    static inline bool apply(Box const& b1, Indexable const& b2)
    {
        return index::get<min_corner, DimensionIndex>(b1) <= index::get<min_corner, DimensionIndex>(b2);
    }
};

template <size_t DimensionIndex, typename Indexable>
struct within_compare<max_corner, DimensionIndex, Indexable, box_tag>
{
    template <typename Box>
    static inline bool apply(Box const& b1, Indexable const& b2)
    {
        return index::get<max_corner, DimensionIndex>(b2) <= index::get<max_corner, DimensionIndex>(b1);
    }
};

template <size_t DimensionIndex, typename Indexable>
struct within_compare<min_corner, DimensionIndex, Indexable, point_tag>
{
    template <typename Box>
    static inline bool apply(Box const& b, Indexable const& p)
    {
        return index::get<min_corner, DimensionIndex>(b) <= geometry::get<DimensionIndex>(p);
    }
};

template <size_t DimensionIndex, typename Indexable>
struct within_compare<max_corner, DimensionIndex, Indexable, point_tag>
{
    template <typename Box>
    static inline bool apply(Box const& b, Indexable const& p)
    {
        return geometry::get<DimensionIndex>(p) <= index::get<max_corner, DimensionIndex>(b);
    }
};

} // namespace dispatch

namespace detail {

template <typename Box, typename Indexable, size_t CurrentDimension>
struct within_for_each_dimension
{
    BOOST_STATIC_ASSERT(0 < CurrentDimension);
    BOOST_STATIC_ASSERT(CurrentDimension <= traits::dimension<Box>::value);
    BOOST_STATIC_ASSERT(traits::dimension<Indexable>::value == traits::dimension<Box>::value);

    static inline bool apply(Box const& b, Indexable const& i)
    {
        return
            within_for_each_dimension<
                Box,
                Indexable,
                CurrentDimension - 1
            >::apply(b, i) &&
            dispatch::within_compare<
                min_corner,
                CurrentDimension - 1,
                Indexable,
                typename traits::tag<Indexable>::type
            >::apply(b, i) &&
            dispatch::within_compare<
                max_corner,
                CurrentDimension - 1,
                Indexable,
                typename traits::tag<Indexable>::type
            >::apply(b, i);
    }
};

template <typename Box, typename Indexable>
struct within_for_each_dimension<Box, Indexable, 1>
{
    BOOST_STATIC_ASSERT(1 <= traits::dimension<Box>::value);
    BOOST_STATIC_ASSERT(traits::dimension<Indexable>::value == traits::dimension<Box>::value);

    static inline bool apply(Box const& b, Indexable const& i)
    {
        return
            dispatch::within_compare<
                min_corner,
                0,
                Indexable,
                typename traits::tag<Indexable>::type
            >::apply(b, i) &&
            dispatch::within_compare<
                max_corner,
                0,
                Indexable,
                typename traits::tag<Indexable>::type
            >::apply(b, i);
    }
};

} // namespace detail

template <typename Box, typename Indexable>
bool within(Box const& box, Indexable const& i)
{
    return detail::within_for_each_dimension<Box, Indexable, traits::dimension<Box>::value>::apply(box, i);
}

}}} // namespace boost::geometry::index

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_ALGORITHMS_WITHIN_HPP
