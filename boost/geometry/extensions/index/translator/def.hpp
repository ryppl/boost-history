// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.SpatialIndex - default value to bounding object translation
//
// Copyright 2011 Adam Wulkiewicz.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_TRANSLATOR_DEF_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_TRANSLATOR_DEF_HPP

#include <boost/geometry/extensions/index/translator/helpers.hpp>

namespace boost { namespace geometry { namespace index { namespace translator {

namespace dispatch {

// Distinguish between def<Geometry>, def<Iterator> and def<SmartPtr>

// Geometry
template <typename Value, bool IsIterator, bool IsSmartPtr>
struct def
{
    typedef typename detail::extract_bounding_geometry<Value>::type bounding_geometry_type;

    bounding_geometry_type const& operator()(Value const& v) const
    {
        return detail::extract_bounding_geometry<Value>::get(v);
    }
};

// Iterator
template <typename Value, bool IsSmartPtr>
struct def<Value, true, IsSmartPtr>
{
    typedef typename detail::extract_bounding_geometry<typename Value::value_type>::type bounding_geometry_type;

    bounding_geometry_type const& operator()(Value const& v) const
    {
        return detail::extract_bounding_geometry<typename Value::value_type>::get(*v);
    }
};

// SmartPtr
template <typename Value>
struct def<Value, false, true>
{
    typedef typename detail::extract_bounding_geometry<typename Value::element_type>::type bounding_geometry_type;

    bounding_geometry_type const& operator()(Value const& v) const
    {
        return detail::extract_bounding_geometry<typename Value::element_type>::get(*v);
    }
};

} // namespace dispatch

template <typename Value>
struct def
    : public dispatch::def
        <
            Value,
            detail::is_iterator<Value>::value,
            detail::is_smart_ptr<Value>::value
        >
{
};

template <typename Value>
struct def<Value*>
{
    typedef typename detail::extract_bounding_geometry<Value>::type bounding_geometry_type;

    bounding_geometry_type const& operator()(const Value *v) const
    {
        return detail::extract_bounding_geometry<Value>::get(*v);
    }
};

}}}} // namespace boost::geometry::index::translator

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_TRANSLATOR_DEF_HPP
