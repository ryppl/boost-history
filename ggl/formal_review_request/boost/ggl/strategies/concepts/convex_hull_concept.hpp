// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_CONCEPTS_CONVEX_HULL_CONCEPT_HPP
#define GGL_STRATEGIES_CONCEPTS_CONVEX_HULL_CONCEPT_HPP


#include <vector>

#include <boost/concept_check.hpp>


namespace ggl { namespace concept {


/*!
    \brief Checks strategy for convex_hull
    \ingroup concepts
*/
template <typename Strategy>
class ConvexHullStrategy
{

    // 1) must define state_type
    typedef typename Strategy::state_type state_type;

    // 2) must define point_type
    typedef typename Strategy::point_type point_type;

    // 3) must define geometry_type
    typedef typename Strategy::geometry_type geometry_type;

    struct check_methods
    {
        static void apply()
        {
            Strategy const* str;

            state_type* st;
            geometry_type* sp;
            std::vector<point_type> *v;

            // 4) must implement a method apply, iterating over a range
            str->apply(*sp, *st);

            // 5) must implement a method result, with an output iterator
            str->result(*st, std::back_inserter(*v), true);
        }
    };

public :
    BOOST_CONCEPT_USAGE(ConvexHullStrategy)
    {
        check_methods::apply();
    }
};



}} // namespace ggl::concept

#endif // GGL_STRATEGIES_CONCEPTS_CONVEX_HULL_CONCEPT_HPP
