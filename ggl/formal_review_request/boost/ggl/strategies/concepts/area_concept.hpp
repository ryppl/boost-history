// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_CONCEPTS_AREA_CONCEPT_HPP
#define GGL_STRATEGIES_CONCEPTS_AREA_CONCEPT_HPP



#include <boost/concept_check.hpp>


namespace ggl { namespace concept {


/*!
    \brief Checks strategy for area
    \ingroup concepts
*/
template <typename Strategy>
class AreaStrategy
{

    // 1) must define state_type,
    typedef typename Strategy::state_type state_type;

    // 2) must define return_type,
    typedef typename Strategy::return_type return_type;

    // 3) must define point_type, of polygon (segments)
    typedef typename Strategy::segment_point_type spoint_type;

    struct check_methods
    {
        static void apply()
        {
            Strategy const* str;

            // 4) must implement an init-method
            state_type st = str->init();

            // 5) must implement a method apply with the following signature
            spoint_type const* sp;
            str->apply(*sp, *sp, st);

            // 6) must implement a static method result with the following signature
            return_type r = str->result(st);
            boost::ignore_unused_variable_warning(r);
        }
    };

public :
    BOOST_CONCEPT_USAGE(AreaStrategy)
    {
        check_methods::apply();
    }
};



}} // namespace ggl::concept

#endif // GGL_STRATEGIES_CONCEPTS_AREA_CONCEPT_HPP
