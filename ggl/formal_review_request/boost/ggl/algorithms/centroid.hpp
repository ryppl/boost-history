// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_CENTROID_HPP
#define GGL_ALGORITHMS_CENTROID_HPP

#include <cstddef>

#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>

#include <ggl/core/cs.hpp>
#include <ggl/core/exception.hpp>
#include <ggl/core/exterior_ring.hpp>
#include <ggl/core/interior_rings.hpp>

#include <ggl/geometries/concepts/check.hpp>

#include <ggl/strategies/strategies.hpp>
#include <ggl/strategies/concepts/centroid_concept.hpp>
#include <ggl/util/copy.hpp>

/*!
\defgroup centroid centroid calculation
\par Source descriptions:
- OGC description: The mathematical centroid for this Surface as a Point. The 
    result is not guaranteed to be on this Surface.
- From Wikipedia: Informally, it is the "average" of all points
\see http://en.wikipedia.org/wiki/Centroid
\note The "centroid" functions are taking a non const reference to the centroid.
    The "make_centroid" functions return the centroid, the type has to be 
    specified.

\note There are versions where a centroid calculation strategy can be specified
\par Geometries:
- RING: \image html centroid_ring.png
- BOX: the centroid of a 2D or 3D box is the center of the box
- POLYGON \image html centroid_polygon.png
- POINT, LINESTRING, SEGMENT: trying to calculate the centroid will result in a 
    compilation error
*/

namespace ggl
{

class centroid_exception : public ggl::exception
{
public:

    inline centroid_exception()  
    {}

    virtual char const* what() const throw()
    {
        return "GGL Centroid calculation exception";
    }
};



#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace centroid {


template<typename Point, typename PointCentroid, typename Strategy>
struct centroid_point
{
    static inline bool apply(Point const& point, PointCentroid& centroid,
            Strategy const&, typename Strategy::state_type&)
    {
        copy_coordinates(point, centroid);
        return false;
    }
};


template
<
    typename Box, 
    typename Point, 
    std::size_t Dimension,
    std::size_t DimensionCount
>
struct centroid_box_calculator
{
    typedef typename select_coordinate_type
        <
            Box, Point
        >::type coordinate_type;
    static inline void apply(Box const& box, Point& centroid)
    {
        coordinate_type const c1 = get<min_corner, Dimension>(box);
        coordinate_type const c2 = get<max_corner, Dimension>(box);
        coordinate_type m = c1 + c2;
        m /= 2.0;

        set<Dimension>(centroid, m);

        centroid_box_calculator
            <
                Box, Point,
                Dimension + 1, DimensionCount
            >::apply(box, centroid);
    }
};


template<typename Box, typename Point, std::size_t DimensionCount>
struct centroid_box_calculator<Box, Point, DimensionCount, DimensionCount>
{
    static inline void apply(Box const& , Point& )
    {
    }
};


template<typename Box, typename Point, typename Strategy>
struct centroid_box
{
    static inline bool apply(Box const& box, Point& centroid,
            Strategy const&, typename Strategy::state_type&)
    {
        centroid_box_calculator
            <
                Box, Point,
                0, dimension<Box>::type::value
            >::apply(box, centroid);
        return false;
    }
};


/*!
    \brief Calculate the centroid of a ring.
*/
template<typename Ring, typename Point, typename Strategy>
struct centroid_ring
{
    static inline bool apply(Ring const& ring, Point& centroid,
            Strategy const& strategy, typename Strategy::state_type& state)
    {
        typedef typename boost::range_const_iterator<Ring>::type iterator_type;

        // There is one thing where centroid is different from e.g. within.
        // If the ring has only one point, it might make sense that
        // that point is the centroid. So then, pass it twice, as a trick
        // to avoid adding another method.

        switch(boost::size(ring))
        {
            case 0 : // will cause exception in the end
                return true; 
            case 1 :
                Strategy::apply(ring.front(), ring.front(), state);
                return false;
        }

        // Normal behaviour:

        iterator_type it = boost::begin(ring);
        for (iterator_type previous = it++;
            it != boost::end(ring);
            previous = it++)
        {
            Strategy::apply(*previous, *it, state);
        }
        return true;
    }
};


/*!
    \brief Centroid of a polygon.
    \note Because outer ring is clockwise, inners are counter clockwise,
    triangle approach is OK and works for polygons with rings.
*/
template<typename Polygon, typename Point, typename Strategy>
struct centroid_polygon
{
    static inline bool apply(Polygon const& poly, Point& centroid,
            Strategy const& strategy, typename Strategy::state_type& state)
    {
        typedef centroid_ring
            <
                typename ring_type<Polygon>::type, 
                Point,
                Strategy
            > per_ring;

        per_ring::apply(exterior_ring(poly), centroid, strategy, state);

        for (typename boost::range_const_iterator
                <
                    typename interior_type<Polygon>::type
                >::type it = boost::begin(interior_rings(poly));
             it != boost::end(interior_rings(poly)); 
             ++it)
        {
            per_ring::apply(*it, centroid, strategy, state);
        }
        return true; // always check exception
    }
};


}} // namespace detail::centroid
#endif // DOXYGEN_NO_DETAIL


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template 
<
    typename Tag, 
    std::size_t Dimension,
    typename Geometry, 
    typename Point,
    typename Strategy
>
struct centroid {};

template 
<
    std::size_t Dimension, 
    typename Geometry, 
    typename Point, 
    typename Strategy
>
struct centroid<point_tag, Dimension, Geometry, Point, Strategy>
    : detail::centroid::centroid_point<Geometry, Point, Strategy>
{};

template 
<
    std::size_t Dimension, 
    typename Box, 
    typename Point, 
    typename Strategy
>
struct centroid<box_tag, Dimension, Box, Point, Strategy>
    : detail::centroid::centroid_box<Box, Point, Strategy>
{};

template <typename Ring, typename Point, typename Strategy>
struct centroid<ring_tag, 2, Ring, Point, Strategy>
    : detail::centroid::centroid_ring<Ring, Point, Strategy>
{};

template <typename Polygon, typename Point, typename Strategy>
struct centroid<polygon_tag, 2, Polygon, Point, Strategy>
    : detail::centroid::centroid_polygon<Polygon, Point, Strategy>

{};

} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH



/*!
    \brief Calculate centroid using a specified strategy
    \ingroup centroid
    \param geometry the geometry to calculate centroid from
    \param c reference to point which will contain the centroid
    \param strategy Calculation strategy for centroid
    \exception centroid_exception if calculation is not successful, 
        e.g. because polygon didn't contain points
 */
template<typename Geometry, typename Point, typename Strategy>
inline void centroid(Geometry const& geometry, Point& c, 
        Strategy const& strategy)
{
    BOOST_CONCEPT_ASSERT( (ggl::concept::CentroidStrategy<Strategy>) );

    concept::check<const Geometry>();
    concept::check<Point>();

    assert_dimension_equal<Geometry, Point>();
    typedef typename point_type<Geometry>::type point_type;

    typename Strategy::state_type state;

    // Call dispatch apply method. That one returns true if centroid
    // should be taken from state.
    if (dispatch::centroid
        <
            typename tag<Geometry>::type,
            dimension<point_type>::type::value,
            Geometry,
            Point,
            Strategy
        >::apply(geometry, c, strategy, state))
    {
        if (! Strategy::result(state, c))
        {
            throw centroid_exception();
        }
    }
}



/*!
    \brief Calculate centroid
    \ingroup centroid
    \details The function centroid calculates the centroid of a geometry 
        using the default strategy.
    Polygon should be closed, and can be orientated either way
    \param geometry a geometry (e.g. closed ring or polygon)
    \param c reference to point which will contain the centroid
    \exception centroid_exception if calculation is not successful, 
        e.g. because polygon didn't contain points
    \par Example:
    Example showing centroid calculation
    \dontinclude doxygen_examples.cpp
    \skip example_centroid_polygon
    \line {
    \until }
 */
template<typename Geometry, typename Point>
inline void centroid(Geometry const& geometry, Point& c)
{
    concept::check<const Geometry>();
    concept::check<Point>();

    typedef typename point_type<Geometry>::type point_type;
    typedef typename strategy_centroid
        <
            typename cs_tag<point_type>::type,
            Point,
            point_type
        >::type strategy_type;

    centroid(geometry, c, strategy_type());
}

// Versions returning a centroid

/*!
    \brief Calculate and return centroid
    \ingroup centroid
    \param geometry the geometry to calculate centroid from
    \return the centroid
    \exception centroid_exception if calculation is not successful, 
        e.g. because polygon didn't contain points
 */
template<typename Point, typename Geometry>
inline Point make_centroid(Geometry const& geometry)
{
    concept::check<const Geometry>();
    concept::check<Point>();

    Point c;
    centroid(geometry, c);
    return c;
}

/*!
    \brief Calculate and return centroid
    \ingroup centroid
    \param geometry the geometry to calculate centroid from
    \param strategy Calculation strategy for centroid
    \return the centroid
    \exception centroid_exception if calculation is not successful, 
        e.g. because polygon didn't contain points
 */
template<typename Point, typename Geometry, typename Strategy>
inline Point make_centroid(Geometry const& geometry, Strategy const& strategy)
{
    BOOST_CONCEPT_ASSERT( (ggl::concept::CentroidStrategy<Strategy>) );

    concept::check<const Geometry>();
    concept::check<Point>();

    Point c;
    centroid(geometry, c, strategy);
    return c;
}

} // namespace ggl

#endif // GGL_ALGORITHMS_CENTROID_HPP
