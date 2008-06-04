// Geometry Library Point concept unit tests
//
// Copyright Bruno Lalande 2008
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef _GEOMETRY_FUNCTION_REQUIRING_A_POINT_HPP
#define _GEOMETRY_FUNCTION_REQUIRING_A_POINT_HPP


#include <boost/concept/requires.hpp>
#include <geometry/concepts/point_concept.hpp>


namespace geometry
{
	template <typename P>
	BOOST_CONCEPT_REQUIRES(
		((Point<P>)),
		(void))
	function_requiring_a_point(P& p1, const P& p2)
	{
		point_traits<P>::template get<0>(p1) =
			point_traits<const P>::template get<0>(p2);
    }
}


#endif
