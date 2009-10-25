// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_CONCEPTS_DISTANCE_CONCEPT_HPP
#define GGL_STRATEGIES_CONCEPTS_DISTANCE_CONCEPT_HPP

#include <vector>
#include <iterator>

#include <boost/concept_check.hpp>

#include <ggl/geometries/concepts/point_concept.hpp>
#include <ggl/geometries/segment.hpp>


namespace ggl { namespace concept {


/*!
    \brief Checks strategy for point-segment-distance
    \ingroup concepts
*/
template <typename Strategy>
struct PointDistanceStrategy
{
    private :

        // 1) must define point_type
        typedef typename Strategy::point_type ptype;
        BOOST_CONCEPT_ASSERT
            (
                (concept::ConstPoint<ptype>)
            );

        // 2) must define return_type
        typedef typename Strategy::return_type rtype;

        // 3) must implement static method () with arguments
        struct apply_checker
        {
            static void check()
            {
                Strategy s;
                ptype *p1;
                ptype *p2;
                rtype r = s(*p1, *p2);

                boost::ignore_unused_variable_warning(r);
            }
        };

    public :
        BOOST_CONCEPT_USAGE(PointDistanceStrategy)
        {
            apply_checker::check();
        }
};


/*!
    \brief Checks strategy for point-segment-distance
    \ingroup concepts
*/
template <typename Strategy>
struct PointSegmentDistanceStrategy
{
    private :

        // 1) must define point_type
        typedef typename Strategy::point_type ptype;
        BOOST_CONCEPT_ASSERT
            (
                (concept::ConstPoint<ptype>)
            );

        // 2) must define return_type
        typedef typename Strategy::return_type rtype;

        // 3) must define underlying point-distance-strategy
        typedef typename Strategy::point_strategy_type stype;
        BOOST_CONCEPT_ASSERT
            (
                (concept::PointDistanceStrategy<stype>)
            );


        // 4) must implement static method () with arguments
        struct apply_checker
        {
            static void check()
            {
                Strategy s;
                ptype *p1;
                ptype *p2;
                rtype r = s(*p1, ggl::segment<const ptype>(*p1, *p2));
                boost::ignore_unused_variable_warning(r);
            }
        };

    public :
        BOOST_CONCEPT_USAGE(PointSegmentDistanceStrategy)
        {
            apply_checker::check();
        }
};



}} // namespace ggl::concept

#endif // GGL_STRATEGIES_CONCEPTS_DISTANCE_CONCEPT_HPP
