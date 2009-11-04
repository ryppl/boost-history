// Generic Geometry Library
//
// Copyright Barend Gehrels 2009, Geodan, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_MULTI_GEOMETRIES_CONCEPTS_CHECK_HPP
#define GGL_MULTI_GEOMETRIES_CONCEPTS_CHECK_HPP



#include <boost/type_traits/is_const.hpp>

#include <ggl/multi/core/tags.hpp>

#include <ggl/geometries/concepts/check.hpp>
#include <ggl/multi/geometries/concepts/multi_point_concept.hpp>
#include <ggl/multi/geometries/concepts/multi_linestring_concept.hpp>
#include <ggl/multi/geometries/concepts/multi_polygon_concept.hpp>


namespace ggl {



#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{


template <typename Geometry>
struct check<multi_point_tag, Geometry, true>
    : detail::concept_check::check<concept::ConstMultiPoint<Geometry> >
{};


template <typename Geometry>
struct check<multi_point_tag, Geometry, false>
    : detail::concept_check::check<concept::MultiPoint<Geometry> >
{};


template <typename Geometry>
struct check<multi_linestring_tag, Geometry, true>
    : detail::concept_check::check<concept::ConstMultiLinestring<Geometry> >
{};


template <typename Geometry>
struct check<multi_linestring_tag, Geometry, false>
    : detail::concept_check::check<concept::MultiLinestring<Geometry> >
{};


template <typename Geometry>
struct check<multi_polygon_tag, Geometry, true>
    : detail::concept_check::check<concept::ConstMultiPolygon<Geometry> >
{};


template <typename Geometry>
struct check<multi_polygon_tag, Geometry, false>
    : detail::concept_check::check<concept::MultiPolygon<Geometry> >
{};


} // namespace dispatch
#endif


} // namespace ggl


#endif // GGL_MULTI_GEOMETRIES_CONCEPTS_CHECK_HPP
