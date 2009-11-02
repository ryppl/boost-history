// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_MULTI_ITERATORS_RANGE_TYPE_HPP
#define GGL_MULTI_ITERATORS_RANGE_TYPE_HPP


#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>

#include <ggl/multi/core/is_multi.hpp>

#include <ggl/iterators/range_type.hpp>

namespace ggl {

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

// multi-point acts itself as a range
template <typename Geometry>
struct range_type<multi_point_tag, Geometry>
{
    typedef Geometry type;
};


template <typename Geometry>
struct range_type<multi_linestring_tag, Geometry>
{
    typedef typename boost::range_value<Geometry>::type type;
};


template <typename Geometry>
struct range_type<multi_polygon_tag, Geometry>
{
    // Call its single-version
    typedef typename ggl::ring_type
        <
            typename boost::range_value<Geometry>::type
        >::type type;
};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


} // namespace ggl

#endif // GGL_MULTI_ITERATORS_RANGE_TYPE_HPP
