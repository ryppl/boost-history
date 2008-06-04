// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_SIDE_HPP
#define _GEOMETRY_SIDE_HPP

#include <geometry/util/promotion_traits.hpp>
#include <geometry/concepts/point_traits.hpp>

namespace geometry
{

	// Check at which side of a segment a point lies:
	// left of segment (> 0), right of segment (< 0), on segment (0)
	// In fact this is twice the area of a triangle
	template <typename S, typename P>
	inline typename select_type_traits<typename point_traits<S>::coordinate_type, 
			typename point_traits<P>::coordinate_type>::type
		point_side(const S& s, const P &p)
	{
		typedef typename select_type_traits<typename point_traits<S>::coordinate_type, 
			typename point_traits<P>::coordinate_type>::type T;

		// Todo: might be changed to subtract_point
		T dx = get<0>(s.second) - get<0>(s.first);
		T dy = get<1>(s.second) - get<1>(s.first);
		T dpx = get<0>(p) - get<0>(s.first);
		T dpy = get<1>(p) - get<1>(s.first);
		return dx * dpy - dy * dpx;
	}


} // namespace geometry

#endif // _GEOMETRY_SIDE_HPP
