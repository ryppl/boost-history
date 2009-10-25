// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_GEOMETRIES_CONCEPTS_POLYGON_CONCEPT_HPP
#define GGL_GEOMETRIES_CONCEPTS_POLYGON_CONCEPT_HPP

#include <boost/concept_check.hpp>
#include <boost/range/concepts.hpp>

#include <ggl/core/access.hpp>
#include <ggl/core/exterior_ring.hpp>
#include <ggl/core/interior_rings.hpp>
#include <ggl/core/point_type.hpp>
#include <ggl/core/ring_type.hpp>

#include <ggl/geometries/concepts/point_concept.hpp>
#include <ggl/geometries/concepts/ring_concept.hpp>

namespace ggl { namespace concept {


/*!
    \brief Checks polygon concept
    \ingroup concepts
*/
template <typename Geometry>
class Polygon
{
    typedef typename point_type<Geometry>::type point_type;
    typedef typename ring_type<Geometry>::type ring_type;
    typedef typename interior_type<Geometry>::type interior_type;

    BOOST_CONCEPT_ASSERT( (concept::Point<point_type>) );
    BOOST_CONCEPT_ASSERT( (concept::Ring<ring_type>) );

    BOOST_CONCEPT_ASSERT( (boost::RandomAccessRangeConcept<interior_type>) );

    struct checker
    {
        static inline void apply()
        {
            Geometry* poly;
            ring_type& e = exterior_ring(*poly);
            ring_type const& ce = exterior_ring(*poly);

            interior_type& i = interior_rings(*poly);
            interior_type const& ci = interior_rings(*poly);

            boost::ignore_unused_variable_warning(e);
            boost::ignore_unused_variable_warning(ce);
            boost::ignore_unused_variable_warning(i);
            boost::ignore_unused_variable_warning(ci);
            boost::ignore_unused_variable_warning(poly);
        }

    };

public:


    BOOST_CONCEPT_USAGE(Polygon)
    {
        // Check if it can be modified
        // TODO

        //Geometry* poly;
        //clear(*poly);
        //append(*poly, point_type());

        checker::apply();
    }
};


/*!
    \brief Checks Polygon concept (const version)
    \ingroup concepts
    \details The ConstPolygon concept check the same as the Polygon concept,
    but does not check write access.
*/
template <typename Geometry>
class ConstPolygon
{

    typedef typename point_type<Geometry>::type point_type;
    typedef typename ring_type<Geometry>::type ring_type;
    typedef typename interior_type<Geometry>::type interior_type;

    BOOST_CONCEPT_ASSERT( (concept::ConstPoint<point_type>) );
    BOOST_CONCEPT_ASSERT( (concept::ConstRing<ring_type>) );

    BOOST_CONCEPT_ASSERT( (boost::RandomAccessRangeConcept<interior_type>) );

    struct checker
    {
        static inline void apply()
        {
            Geometry* poly;
            ring_type const& ce = exterior_ring(*poly);
            interior_type const& ci = interior_rings(*poly);

            boost::ignore_unused_variable_warning(ce);
            boost::ignore_unused_variable_warning(ci);
            boost::ignore_unused_variable_warning(poly);
        }
    };


public:

    BOOST_CONCEPT_USAGE(ConstPolygon)
    {
        checker::apply();
    }
};

}} // namespace ggl::concept

#endif // GGL_GEOMETRIES_CONCEPTS_POLYGON_CONCEPT_HPP
