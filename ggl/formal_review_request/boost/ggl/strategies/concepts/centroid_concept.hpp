// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_CONCEPTS_CENTROID_CONCEPT_HPP
#define GGL_STRATEGIES_CONCEPTS_CENTROID_CONCEPT_HPP



#include <boost/concept_check.hpp>


namespace ggl { namespace concept {


/*!
    \brief Checks strategy for area
    \ingroup concepts
*/
template <typename Strategy>
class CentroidStrategy
{

    // 1) must define state_type,
    typedef typename Strategy::state_type state_type;

    // 2) must define point_type,
    typedef typename Strategy::point_type point_type;

    // 3) must define point_type, of polygon (segments)
    typedef typename Strategy::segment_point_type spoint_type;

    struct check_methods
    {
        static void apply()
        {
            state_type *s;

            // 4) must implement a static method apply,
            // getting two segment-points
            spoint_type *sp;
            Strategy::apply(*sp, *sp, *s);

            // 5) must implement a static method result
            //  getting the centroid
            point_type *c;
            bool r = Strategy::result(*s, *c);
            boost::ignore_unused_variable_warning(r);
        }
    };

public :
    BOOST_CONCEPT_USAGE(CentroidStrategy)
    {
        check_methods::apply();
    }
};



}} // namespace ggl::concept

#endif // GGL_STRATEGIES_CONCEPTS_CENTROID_CONCEPT_HPP
