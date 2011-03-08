// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.SpatialIndex - box query filter implementation
//
// Copyright 2011 Adam Wulkiewicz.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_FILTERS_SPACIAL_FILTER_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_FILTERS_SPACIAL_FILTER_HPP

namespace boost { namespace geometry { namespace index { namespace filters {

template <typename SpacialIndex>
class spatial_filter
{
public:
    typedef int* iterator;
    typedef const int* const_iterator;

    template <typename Box>
    spatial_filter(SpacialIndex const&, Box const&) {}

    iterator begin() { return 0; }
    iterator end() { return 0; }
    const_iterator begin() const { return 0; }
    const_iterator end() const { return 0; }
};

namespace detail {

template<typename Box>
class spatially_filtered
{
public:
    explicit spatially_filtered(Box const& b) : m_box(b) {}
    Box const& box() const { return m_box; }

private:
    Box m_box;
};

} // namespace detail

template <typename Box>
detail::spatially_filtered<Box> spatially_filtered(Box const& b)
{
    return detail::spatially_filtered<Box>(b);
}

}}}} // namespace boost::geometry::index::filters

template<typename SpacialIndex, typename Box>
boost::geometry::index::filters::spatial_filter<SpacialIndex>
operator|(
    SpacialIndex const& si,
    boost::geometry::index::filters::detail::spatially_filtered<Box> const& f)
{
    return boost::geometry::index::filters::spatial_filter<SpacialIndex>(si, f.box());
}

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_FILTERS_SPACIAL_FILTER_HPP
