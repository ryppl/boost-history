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




namespace ggl { namespace concept {

/*!
\brief Point concept
\ingroup concepts

\par Formal definition:
The point concept is defined as following:
- there must be a specialization of traits::tag defining point_tag as type
- there must be a specialization of traits::coordinate_type defining the type
  of its coordinates
- there must be a specialization of traits::coordinate_system defining its
  coordinate system (cartesian, spherical, etc)
- there must be a specialization of traits::dimension defining its number
  of dimensions (2, 3, ...) (derive it conveniently
  from boost::mpl::int_&lt;X&gt; for X-D)
- there must be a specialization of traits::access, per dimension,
  with two functions:
  - \b get to get a coordinate value
  - \b set to set a coordinate value (this one is not checked for ConstPoint)

\par Example:

A legacy point, defining the necessary specializations to fulfil to the concept.

Suppose that the following point is defined:
\dontinclude doxygen_5.cpp
\skip legacy_point1
\until };

It can then be adapted to the concept as following:
\dontinclude doxygen_5.cpp
\skip adapt legacy_point1
\until }}

Note that it is done like above to show the system. Users will normally use the registration macro.

\par Example:

A read-only legacy point, using a macro to fulfil to the ConstPoint concept.
It cannot be modified by the library but can be used in all algorithms where
points are not modified.

The point looks like the following:

\dontinclude doxygen_5.cpp
\skip legacy_point2
\until };

It uses the macro as following:
\dontinclude doxygen_5.cpp
\skip adapt legacy_point2
\until end adaptation

*/

template <typename Geometry>
class Point
{
#ifndef DOXYGEN_NO_CONCEPT_MEMBERS

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
#endif
};


/*!
\brief point concept (const version)
\ingroup const_concepts
\details The ConstPoint concept apply the same as the Point concept,
but does not apply write access.
*/
template <typename Geometry>
class ConstPoint
{
#ifndef DOXYGEN_NO_CONCEPT_MEMBERS

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
#endif
};

}} // namespace ggl::concept

#endif // GGL_GEOMETRIES_CONCEPTS_POINT_CONCEPT_HPP