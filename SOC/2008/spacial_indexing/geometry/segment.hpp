// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_SEGMENT_HPP
#define _GEOMETRY_SEGMENT_HPP

#include <geometry/geometry.hpp>

namespace geometry
{

	/*! 
		\brief Class segment: small containing two (templatized) point references
		\details From Wikipedia: In geometry, a line segment is a part of a line that is bounded 
		by two distinct end points, and contains every point on the line between its end points
		\note The structure is like std::pair, and can often be used interchangeable.
		So points are public available. We cannot derive from std::pair<P&, P&> because of reference 
		assignments. Points are not const and might be changed by the algorithm
		(used in intersection_linestring)
		\par Template parameters:
		- \a P point type of the circle center
	 */
	template<typename P>
	struct segment : public geometry_traits<P>
	{
		P& first;
		P& second;
		inline segment(P& p1, P& p2)
			: first(p1), second(p2)
		{}
	};

	/*! 
		\brief Class const_segment: small containing two (templatized) point references
		\details Same as segment, but const (used in intersection_polygon, used in distance)
		\par Template parameters:
		- \a P point type of the circle center
	 */
	template<typename P>
	struct const_segment : public geometry_traits<P>
	{
		const P& first;
		const P& second;
		inline const_segment(const P& p1, const P& p2)
			: first(p1), second(p2)
		{}
	};


} // namespace geometry


#endif //_GEOMETRY_SEGMENT_HPP
