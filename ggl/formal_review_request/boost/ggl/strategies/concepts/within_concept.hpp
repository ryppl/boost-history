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

    // 4) must implement a static method apply with the following signature
    typedef bool (*ApplyType)(point_type const&,
            spoint_type const&, spoint_type const&,
            state_type &);

    // 5) must implement a static method result with the following signature
    typedef bool (*ResultType)(state_type const&) ;

public :
    BOOST_CONCEPT_USAGE(WithinStrategy)
    {
        ApplyType a = &Strategy::apply;
        ResultType r = &Strategy::result;

        boost::ignore_unused_variable_warning(a);
        boost::ignore_unused_variable_warning(r);
    }
};



}} // namespace ggl::concept

#endif // GGL_STRATEGIES_CONCEPTS_WITHIN_CONCEPT_HPP
