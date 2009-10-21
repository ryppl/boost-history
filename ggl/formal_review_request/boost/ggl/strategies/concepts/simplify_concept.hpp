// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_CONCEPTS_SIMPLIFY_CONCEPT_HPP
#define GGL_STRATEGIES_CONCEPTS_SIMPLIFY_CONCEPT_HPP

#include <vector>
#include <iterator>

#include <boost/concept_check.hpp>

#include <ggl/strategies/concepts/distance_concept.hpp>


namespace ggl { namespace concept {


/*!
    \brief Checks strategy for simplify
    \ingroup concepts
*/
template <typename Strategy>
struct SimplifyStrategy
{
    private :

        // 1) must define distance_strategy_type,
        //    defining point-segment distance strategy (to be checked)
        typedef typename Strategy::distance_strategy_type ds_type;

        BOOST_CONCEPT_ASSERT
            (
                (concept::PointSegmentDistanceStrategy<ds_type>)
            );

        // 2) must implement static method apply with arguments
        //    - Range
        //    - OutputIterator
        //    - floating point value
        struct apply_checker
        {
            static void check()
            {
                std::vector<typename ds_type::point_type> *v;
                Strategy::apply(*v, std::back_inserter(*v), 1.0);
            }
        };

    public :
        BOOST_CONCEPT_USAGE(SimplifyStrategy)
        {
            apply_checker::check();

        }
};



}} // namespace ggl::concept

#endif // GGL_STRATEGIES_CONCEPTS_SIMPLIFY_CONCEPT_HPP
