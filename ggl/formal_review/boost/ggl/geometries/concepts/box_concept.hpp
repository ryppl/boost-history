// Generic Geometry Library
//
// Copyright Bruno Lalande 2008, 2009
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_GEOMETRIES_CONCEPTS_BOX_CONCEPT_HPP
#define GGL_GEOMETRIES_CONCEPTS_BOX_CONCEPT_HPP


#include <cstddef>

#include <boost/concept_check.hpp>


#include <ggl/core/access.hpp>
#include <ggl/core/coordinate_dimension.hpp>
#include <ggl/core/point_type.hpp>



namespace ggl { namespace concept {


/*!
\brief Box concept
\ingroup concepts
\par Formal definition:
The box concept is defined as following:
- there must be a specialization of traits::tag defining box_tag as type
- there must be a specialization of traits::point_type to define the
  underlying point type (even if it does not consist of points, it should define
  this type, to indicate the points it can work with)
- there must be a specialization of traits::indexed_access, per index
  (min_corner, max_corner) and per dimension, with two functions:
  - get to get a coordinate value
  - set to set a coordinate value (this one is not checked for ConstBox)
*/
template <typename Geometry>
class Box
{
#ifndef DOXYGEN_NO_CONCEPT_MEMBERS
    typedef typename point_type<Geometry>::type point_type;


    template
    <
        std::size_t Index,
        std::size_t Dimension,
        std::size_t DimensionCount
    >
    struct dimension_checker
    {
        static void apply()
        {
            Geometry* b;
            ggl::set<Index, Dimension>(*b, ggl::get<Index, Dimension>(*b));
            dimension_checker<Index, Dimension + 1, DimensionCount>::apply();
        }
    };

    template <std::size_t Index, std::size_t DimensionCount>
    struct dimension_checker<Index, DimensionCount, DimensionCount>
    {
        static void apply() {}
    };

public :
    BOOST_CONCEPT_USAGE(Box)
    {
        static const std::size_t n = dimension<Geometry>::type::value;
        dimension_checker<min_corner, 0, n>::apply();
        dimension_checker<max_corner, 0, n>::apply();
    }
#endif
};


/*!
\brief Box concept (const version)
\ingroup const_concepts
\details The ConstBox concept apply the same as the Box concept,
but does not apply write access.
*/
template <typename Geometry>
class ConstBox
{
#ifndef DOXYGEN_NO_CONCEPT_MEMBERS
    typedef typename point_type<Geometry>::type point_type;
    typedef typename coordinate_type<Geometry>::type coordinate_type;

    template
    <
        std::size_t Index,
        std::size_t Dimension,
        std::size_t DimensionCount
    >
    struct dimension_checker
    {
        static void apply()
        {
            const Geometry* b = 0;
            coordinate_type coord(ggl::get<Index, Dimension>(*b));
            boost::ignore_unused_variable_warning(coord);
            dimension_checker<Index, Dimension + 1, DimensionCount>::apply();
        }
    };

    template <std::size_t Index, std::size_t DimensionCount>
    struct dimension_checker<Index, DimensionCount, DimensionCount>
    {
        static void apply() {}
    };

public :
    BOOST_CONCEPT_USAGE(ConstBox)
    {
        static const std::size_t n = dimension<Geometry>::type::value;
        dimension_checker<min_corner, 0, n>::apply();
        dimension_checker<max_corner, 0, n>::apply();
    }
#endif
};

}} // namespace ggl::concept


#endif // GGL_GEOMETRIES_CONCEPTS_BOX_CONCEPT_HPP
