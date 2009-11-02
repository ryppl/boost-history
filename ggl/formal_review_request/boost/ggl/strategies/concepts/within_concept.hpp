// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_CONCEPTS_WITHIN_CONCEPT_HPP
#define GGL_STRATEGIES_CONCEPTS_WITHIN_CONCEPT_HPP



#include <boost/concept_check.hpp>


namespace ggl { namespace concept {


/*!
    \brief Checks strategy for within (point-in-polygon)
    \ingroup concepts
*/
template <typename Strategy>
class WithinStrategy
{

    // 1) must define state_type,
    typedef typename Strategy::state_type state_type;

    // 2) must define point_type (of "point" in poly)
    typedef typename Strategy::point_type point_type;

    // 3) must define point_type, of polygon (segments)
    typedef typename Strategy::segment_point_type spoint_type;


    struct check_methods
    {
        static void apply()
        {
            Strategy const* str;

            state_type* st;
            point_type const* p;
            spoint_type const* sp;

            // 4) must implement a method apply
            //    having a point, two segment-points, and state
            str->apply(*p, *sp, *sp, *st);

            // 5) must implement a method result
            str->result(*st);
            boost::ignore_unused_variable_warning(str);
        }
    };


public :
    BOOST_CONCEPT_USAGE(WithinStrategy)
    {
        check_methods::apply();
    }
};



}} // namespace ggl::concept

#endif // GGL_STRATEGIES_CONCEPTS_WITHIN_CONCEPT_HPP
