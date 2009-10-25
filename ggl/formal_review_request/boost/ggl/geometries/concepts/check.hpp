// Generic Geometry Library
//
// Copyright Barend Gehrels 2009, Geodan, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_GEOMETRIES_CONCEPTS_CHECK_HPP
#define GGL_GEOMETRIES_CONCEPTS_CHECK_HPP


#include <boost/concept_check.hpp>
#include <boost/concept/requires.hpp>

#include <boost/type_traits/is_const.hpp>

#include <ggl/core/tag.hpp>

#include <ggl/geometries/concepts/box_concept.hpp>
#include <ggl/geometries/concepts/linestring_concept.hpp>
#include <ggl/geometries/concepts/point_concept.hpp>
#include <ggl/geometries/concepts/polygon_concept.hpp>
#include <ggl/geometries/concepts/ring_concept.hpp>


namespace ggl {


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace concept_check {

template <typename Concept>
class check
{
    BOOST_CONCEPT_ASSERT((Concept ));
};

}} // namespace detail::check
#endif // DOXYGEN_NO_DETAIL



#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename GeometryTag, typename Geometry, bool IsConst>
struct check
{};


template <typename Geometry>
struct check<point_tag, Geometry, true>
    : detail::concept_check::check<concept::ConstPoint<Geometry> >
{};


template <typename Geometry>
struct check<point_tag, Geometry, false>
    : detail::concept_check::check<concept::Point<Geometry> >
{};


template <typename Geometry>
struct check<linestring_tag, Geometry, true>
    : detail::concept_check::check<concept::ConstLinestring<Geometry> >
{};


template <typename Geometry>
struct check<linestring_tag, Geometry, false>
    : detail::concept_check::check<concept::Linestring<Geometry> >
{};


template <typename Geometry>
struct check<polygon_tag, Geometry, true>
    : detail::concept_check::check<concept::ConstPolygon<Geometry> >
{};


template <typename Geometry>
struct check<polygon_tag, Geometry, false>
    : detail::concept_check::check<concept::Polygon<Geometry> >
{};


template <typename Geometry>
struct check<box_tag, Geometry, true>
    : detail::concept_check::check<concept::ConstBox<Geometry> >
{};


template <typename Geometry>
struct check<box_tag, Geometry, false>
    : detail::concept_check::check<concept::Box<Geometry> >
{};



} // namespace dispatch
#endif




namespace concept {


namespace detail {

/*!
    \brief Checks, in compile-time, the concept of any geometry
    \ingroup core
*/
template <typename Geometry, bool IsConst>
struct checker : dispatch::check
    <
        typename tag<Geometry>::type,
        Geometry,
        IsConst
    >
{};

}


/*!
    \brief Checks, in compile-time, the concept of any geometry
    \ingroup core
*/
template <typename Geometry>
inline void check()
{
    detail::checker<Geometry, boost::is_const<Geometry>::type::value> c;
    boost::ignore_unused_variable_warning(c);
}



} // namespace concept


} // namespace ggl


#endif // GGL_GEOMETRIES_CONCEPTS_CHECK_HPP
