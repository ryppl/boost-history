// Generic Geometry Library
//
// Copyright Bruno Lalande 2008, 2009
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_GEOMETRIES_CONCEPTS_SEGMENT_CONCEPT_HPP
#define GGL_GEOMETRIES_CONCEPTS_SEGMENT_CONCEPT_HPP


#include <boost/concept_check.hpp>

#include <ggl/geometries/concepts/point_concept.hpp>



#include <ggl/core/access.hpp>
#include <ggl/core/point_type.hpp>



namespace ggl { namespace concept {


/*!
    \brief Checks segment concept
    \ingroup concepts
*/
template <typename Geometry>
class Segment
{
    typedef typename point_type<Geometry>::type point_type;

    BOOST_CONCEPT_ASSERT( (concept::Point<point_type>) );


    template <size_t Index, size_t Dimension, size_t DimensionCount>
    struct dimension_checker
    {
        static void apply()
        {
            Geometry* s;
            ggl::set<Index, Dimension>(*s, ggl::get<Index, Dimension>(*s));
            dimension_checker<Index, Dimension + 1, DimensionCount>::apply();
        }
    };

    template <size_t Index, size_t DimensionCount>
    struct dimension_checker<Index, DimensionCount, DimensionCount>
    {
        static void apply() {}
    };

public :

    BOOST_CONCEPT_USAGE(Segment)
    {
        static const size_t n = dimension<point_type>::type::value;
        dimension_checker<0, 0, n>::apply();
        dimension_checker<1, 0, n>::apply();
    }
};


/*!
    \brief Checks Segment concept (const version)
    \ingroup concepts
    \details The ConstSegment concept verifies the same as the Segment concept,
    but does not verify write access.
*/
template <typename Geometry>
class ConstSegment
{
    typedef typename point_type<Geometry>::type point_type;
    typedef typename coordinate_type<Geometry>::type coordinate_type;

    BOOST_CONCEPT_ASSERT( (concept::ConstPoint<point_type>) );


    template <size_t Index, size_t Dimension, size_t DimensionCount>
    struct dimension_checker
    {
        static void apply()
        {
            const Geometry* s = 0;
            coordinate_type coord(ggl::get<Index, Dimension>(*s));
            boost::ignore_unused_variable_warning(coord);
            dimension_checker<Index, Dimension + 1, DimensionCount>::apply();
        }
    };

    template <size_t Index, size_t DimensionCount>
    struct dimension_checker<Index, DimensionCount, DimensionCount>
    {
        static void apply() {}
    };

public :

    BOOST_CONCEPT_USAGE(ConstSegment)
    {
        static const size_t n = dimension<point_type>::type::value;
        dimension_checker<0, 0, n>::apply();
        dimension_checker<1, 0, n>::apply();
    }
};


}} // namespace ggl::concept


#endif // GGL_GEOMETRIES_CONCEPTS_SEGMENT_CONCEPT_HPP
