// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_FOR_EACH_HPP
#define GGL_ALGORITHMS_FOR_EACH_HPP

/*!
\defgroup loop loops and for-each functionality
There are several algorithms provided which walk through the points or segments
of linestrings and polygons. They are called for_each_point, for_each_segment, after
the standard library, and \b loop which is more adapted and of which the functor
could break out if necessary.
Of the for_each algorithms there is a \b const and a non-const version provided.
*/

#include <algorithm>

#include <boost/concept/requires.hpp>
#include <boost/mpl/if.hpp>
#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>

#include <ggl/core/exterior_ring.hpp>
#include <ggl/core/interior_rings.hpp>
#include <ggl/core/is_multi.hpp>
#include <ggl/core/concepts/point_concept.hpp>
#include <ggl/geometries/segment.hpp>
#include <ggl/iterators/vertex_iterator.hpp>

namespace ggl
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace for_each {


// There is probably something in boost type_traits
// or MPL with this functionality
template <typename Type, bool IsConst>
struct c_nc
{
    typedef typename boost::mpl::if_c
        <
            IsConst,
            const Type,
            Type
        >::type type;
};

// Utility to adapt a boost-range for const/non const
template <typename Range, bool IsConst>
struct c_nc_range
{
    typedef typename boost::mpl::if_c
        <
            IsConst,
            typename boost::range_const_iterator<Range>::type,
            typename boost::range_iterator<Range>::type
        >::type type;
};


template <typename Point, typename Functor, bool IsConst>
struct fe_point_per_point
{
    static inline Functor apply(
                typename c_nc<Point, IsConst>::type& point, Functor f)
    {
        f(point);
        return f;
    }
};

template <typename Point, typename Functor, bool IsConst>
struct fe_point_per_segment
{
    static inline Functor apply(
                typename c_nc<Point, IsConst>::type& point, Functor f)
    {
        return f;
    }
};

template <typename Range, typename Functor, bool IsConst>
struct fe_range_per_point
{
    static inline Functor apply(
                    typename c_nc<Range, IsConst>::type& range,
                    Functor f)
    {
        return (std::for_each(boost::begin(range), boost::end(range), f));
    }
};

template <typename Range, typename Functor, bool IsConst>
struct fe_range_per_segment
{
    static inline Functor apply(
                typename c_nc<Range, IsConst>::type& range,
                Functor f)
    {
        typedef typename ggl::vertex_iterator<Range, IsConst>::type iterator_type;

        typedef typename c_nc
            <
                typename point_type<Range>::type, IsConst
            >::type point_type;

        iterator_type it = boost::begin(range);
        iterator_type previous = it++;
        while(it != boost::end(range))
        {
            segment<point_type> s(*previous, *it);
            f(s);
            previous = it++;
        }

        return f;
    }
};


template <typename Polygon, typename Functor, bool IsConst>
struct fe_polygon_per_point
{
    static inline Functor apply(
                typename c_nc<Polygon, IsConst>::type& poly,
                Functor f)
    {
        typedef typename c_nc_range
            <
                typename interior_type<Polygon>::type, 
                IsConst
            >::type iterator_type;

        typedef fe_range_per_point
                <
                    typename ring_type<Polygon>::type, 
                    Functor, 
                    IsConst
                > per_ring;

        f = per_ring::apply(exterior_ring(poly), f);

        for (iterator_type it = boost::begin(interior_rings(poly));
             it != boost::end(interior_rings(poly));
             ++it)
        {
            f = per_ring::apply(*it, f);
        }

        return f;
    }

};


template <typename Polygon, typename Functor, bool IsConst>
struct fe_polygon_per_segment
{
    static inline Functor apply(
                typename c_nc<Polygon, IsConst>::type& poly,
                Functor f)
    {
        typedef typename c_nc_range
            <
                typename interior_type<Polygon>::type, 
                IsConst
            >::type iterator_type;

        typedef fe_range_per_segment
            <
                typename ring_type<Polygon>::type, 
                Functor, 
                IsConst
            > per_ring;

        f = per_ring::apply(exterior_ring(poly), f);

        for (iterator_type it = boost::begin(interior_rings(poly));
             it != boost::end(interior_rings(poly));
             ++it)
        {
            f = per_ring::apply(*it, f);
        }

        return f;
    }

};

}} // namespace detail::for_each
#endif // DOXYGEN_NO_DETAIL


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template 
<
    typename Tag, 
    bool IsMulti, 
    typename Geometry, 
    typename Functor, 
    bool IsConst
>
struct for_each_point {};

template <typename Point, typename Functor, bool IsConst>
struct for_each_point<point_tag, false, Point, Functor, IsConst>
    : detail::for_each::fe_point_per_point<Point, Functor, IsConst>
{};

template <typename Linestring, typename Functor, bool IsConst>
struct for_each_point<linestring_tag, false, Linestring, Functor, IsConst>
    : detail::for_each::fe_range_per_point<Linestring, Functor, IsConst>
{};

template <typename Ring, typename Functor, bool IsConst>
struct for_each_point<ring_tag, false, Ring, Functor, IsConst>
    : detail::for_each::fe_range_per_point<Ring, Functor, IsConst>
{};

template <typename Polygon, typename Functor, bool IsConst>
struct for_each_point<polygon_tag, false, Polygon, Functor, IsConst>
    : detail::for_each::fe_polygon_per_point<Polygon, Functor, IsConst>
{};




template 
<
    typename Tag, 
    bool IsMulti, 
    typename Geometry, 
    typename Functor, 
    bool IsConst
>
struct for_each_segment {};

template <typename Point, typename Functor, bool IsConst>
struct for_each_segment<point_tag, false, Point, Functor, IsConst>
    : detail::for_each::fe_point_per_segment<Point, Functor, IsConst>
{};

template <typename Linestring, typename Functor, bool IsConst>
struct for_each_segment<linestring_tag, false, Linestring, Functor, IsConst>
    : detail::for_each::fe_range_per_segment<Linestring, Functor, IsConst>
{};

template <typename Ring, typename Functor, bool IsConst>
struct for_each_segment<ring_tag, false, Ring, Functor, IsConst>
    : detail::for_each::fe_range_per_segment<Ring, Functor, IsConst>
{};

template <typename Polygon, typename Functor, bool IsConst>
struct for_each_segment<polygon_tag, false, Polygon, Functor, IsConst>
    : detail::for_each::fe_polygon_per_segment<Polygon, Functor, IsConst>
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH

/*!
    \brief Calls functor for geometry
    \ingroup loop
    \param geometry geometry to loop through
    \param f functor to use
    \details Calls the functor the specified \b const geometry
*/
template<typename Geometry, typename Functor>
inline Functor for_each_point(Geometry const& geometry, Functor f)
{
    return dispatch::for_each_point
        <
            typename tag<Geometry>::type, 
            is_multi<Geometry>::type::value,
            Geometry, 
            Functor, 
            true
        >::apply(geometry, f);
}

/*!
    \brief Calls functor for geometry
    \ingroup loop
    \param geometry geometry to loop through
    \param f functor to use
    \details Calls the functor for the specified geometry
*/
template<typename Geometry, typename Functor>
inline Functor for_each_point(Geometry& geometry, Functor f)
{
    return dispatch::for_each_point
        <
            typename tag<Geometry>::type, 
            is_multi<Geometry>::type::value,
            Geometry, 
            Functor, 
            false
        >::apply(geometry, f);
}

/*!
    \brief Calls functor for segments on linestrings, rings, polygons, ...
    \ingroup loop
    \param geometry geometry to loop through
    \param f functor to use
    \details Calls the functor all \b const segments of the specified \b const geometry
*/
template<typename Geometry, typename Functor>
inline Functor for_each_segment(Geometry const& geometry, Functor f)
{
    return dispatch::for_each_segment
        <
            typename tag<Geometry>::type, 
            is_multi<Geometry>::type::value,
            Geometry, 
            Functor, 
            true
        >::apply(geometry, f);
}


/*!
    \brief Calls functor for segments on linestrings, rings, polygons, ...
    \ingroup loop
    \param geometry geometry to loop through
    \param f functor to use
    \details Calls the functor all segments of the specified geometry
*/
template<typename Geometry, typename Functor>
inline Functor for_each_segment(Geometry& geometry, Functor f)
{
    return dispatch::for_each_segment
        <
            typename tag<Geometry>::type, 
            is_multi<Geometry>::type::value,
            Geometry, 
            Functor, 
            false
        >::apply(geometry, f);
}

} // namespace ggl

#endif // GGL_ALGORITHMS_FOR_EACH_HPP
