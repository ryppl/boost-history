// Generic Geometry Library Point concept unit tests
//
// Copyright Bruno Lalande 2008, 2009
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_POINT_CONCEPT_FUNCTION_REQUIRING_A_POINT_HPP
#define GGL_TEST_POINT_CONCEPT_FUNCTION_REQUIRING_A_POINT_HPP

#include <boost/concept/requires.hpp>

#include <ggl/geometries/concepts/point_concept.hpp>

namespace ggl
{
    template <typename P, typename C>

    inline BOOST_CONCEPT_REQUIRES(
        ((concept::Point<P>))
        ((concept::ConstPoint<C>)),
        (void))
    function_requiring_a_point(P& p1, const C& p2)
    {
        set<0>(p1, get<0>(p2));
    }
}

#endif // GGL_TEST_POINT_CONCEPT_FUNCTION_REQUIRING_A_POINT_HPP
