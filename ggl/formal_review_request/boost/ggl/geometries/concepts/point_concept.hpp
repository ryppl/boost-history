// Generic Geometry Library Point concept
//
// Copyright Bruno Lalande 2008, 2009
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_GEOMETRIES_CONCEPTS_POINT_CONCEPT_HPP
#define GGL_GEOMETRIES_CONCEPTS_POINT_CONCEPT_HPP

#include <cstddef>

#include <boost/concept_check.hpp>

#include <ggl/core/access.hpp>
#include <ggl/core/coordinate_dimension.hpp>
#include <ggl/core/coordinate_system.hpp>


/*!
\defgroup concepts concept checking
Concepts are used to apply if pointtypes provide required implementation. Concept checking
is done using BCCL (Boost Concept Check Library) and MPL (Meta Programming Library)
*/

namespace ggl { namespace concept {

/*!
    \brief Checks point concept
    \ingroup concepts
    \details The concept is separated into 4 metafunctions:
    - \ref ggl::traits::coordinate_type "coordinate_type": provides the type of the coordinates of a point
    - \ref ggl::traits::coordinate_system "coordinate system": provides the coordinate system in which the point is placed
    - \ref ggl::traits::dimension "dimension": provides the number of coordinates of a point
    - \ref ggl::traits::access "access": provides access to the coordinates of a point

    In MPL, a metafunction that provides a type must expose is as "type"
    and a metafunction that provides a value must expose it as "value", so
    here the same convention are used: coordinate_type<P>::type and
    dimension<P>::value provide the type and number of coordinates. This
    makes them compatible with any MPL and Fusion algorithm and
    metafunction.

    \par Example:
    First example, using an own pointtype, for example a legacy point, defining the necessary
    properties outside the pointtype in a traits class
    \dontinclude doxygen_examples.cpp
    \skip example_point_1
    \until //:\\
    \par Example:
    Second example, deriving a pointtype from boost::tuple. It defines the necessary properties
    itself, so a separate traits class is not necessary.
    \dontinclude doxygen_examples.cpp
    \skip example_own_point2
    \line {
    \until //:\\
*/

template <typename Geometry>
class Point
{

    typedef typename coordinate_type<Geometry>::type ctype;
    typedef typename coordinate_system<Geometry>::type csystem;

    enum { ccount = dimension<Geometry>::value };


    template <typename P, std::size_t Dimension, std::size_t DimensionCount>
    struct dimension_checker
    {
        static void apply()
        {
            P* p;
            ggl::set<Dimension>(*p, ggl::get<Dimension>(*p));
            dimension_checker<P, Dimension+1, DimensionCount>::apply();
        }
    };


    template <typename P, std::size_t DimensionCount>
    struct dimension_checker<P, DimensionCount, DimensionCount>
    {
        static void apply() {}
    };

public:

    /// BCCL macro to apply the Point concept
    BOOST_CONCEPT_USAGE(Point)
    {
        dimension_checker<Geometry, 0, ccount>::apply();
    }
};


/*!
    \brief Checks point concept (const version)
    \ingroup concepts
    \details The ConstPoint concept apply the same as the Point concept,
    but does not apply write access.
*/
template <typename Geometry>
class ConstPoint
{

    typedef typename coordinate_type<Geometry>::type ctype;
    typedef typename coordinate_system<Geometry>::type csystem;

    enum { ccount = dimension<Geometry>::value };


    template <typename P, std::size_t Dimension, std::size_t DimensionCount>
    struct dimension_checker
    {
        static void apply()
        {
            const P* p = 0;
            ctype coord(ggl::get<Dimension>(*p));
            boost::ignore_unused_variable_warning(coord);
            dimension_checker<P, Dimension+1, DimensionCount>::apply();
        }
    };


    template <typename P, std::size_t DimensionCount>
    struct dimension_checker<P, DimensionCount, DimensionCount>
    {
        static void apply() {}
    };

public:

    /// BCCL macro to apply the ConstPoint concept
    BOOST_CONCEPT_USAGE(ConstPoint)
    {
        dimension_checker<Geometry, 0, ccount>::apply();
    }
};

}} // namespace ggl::concept

#endif // GGL_GEOMETRIES_CONCEPTS_POINT_CONCEPT_HPP
