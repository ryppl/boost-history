// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_GEOMETRIES_CONCEPTS_RING_CONCEPT_HPP
#define GGL_GEOMETRIES_CONCEPTS_RING_CONCEPT_HPP


#include <boost/concept_check.hpp>
#include <boost/range/concepts.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <ggl/core/access.hpp>
#include <ggl/core/point_type.hpp>

#include <ggl/geometries/concepts/point_concept.hpp>

#include <ggl/geometries/concepts/detail/check_clear.hpp>
#include <ggl/geometries/concepts/detail/check_append.hpp>


namespace ggl { namespace concept {


/*!
    \brief Checks (linear) ring concept
    \ingroup concepts
*/
template <typename Geometry>
class Ring
{
    typedef typename point_type<Geometry>::type point_type;

    BOOST_CONCEPT_ASSERT( (concept::Point<point_type>) );
    BOOST_CONCEPT_ASSERT( (boost::RandomAccessRangeConcept<Geometry>) );


public :

    BOOST_CONCEPT_USAGE(Ring)
    {
        // Check if it can be modified
        static const bool use_std = traits::use_std
            <
                typename boost::remove_const<Geometry>::type
            >::value;

        Geometry* ring;
        detail::check_clear<Geometry, use_std>::apply(*ring);

        point_type* p;
        detail::check_append<Geometry, point_type, use_std>::apply(*ring, *p);
    }
};


/*!
    \brief Checks (linear) ring concept (const version)
    \ingroup concepts
    \details The ConstLinearRing concept check the same as the Geometry concept,
    but does not check write access.
*/
template <typename Geometry>
class ConstRing
{
    typedef typename point_type<Geometry>::type point_type;

    BOOST_CONCEPT_ASSERT( (concept::ConstPoint<point_type>) );
    BOOST_CONCEPT_ASSERT( (boost::RandomAccessRangeConcept<Geometry>) );


public :

    BOOST_CONCEPT_USAGE(ConstRing)
    {
    }
};

}} // namespace ggl::concept


#endif // GGL_GEOMETRIES_CONCEPTS_RING_CONCEPT_HPP
