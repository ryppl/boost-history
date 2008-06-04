// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_POINT_ON_LINE_HPP
#define _GEOMETRY_POINT_ON_LINE_HPP

#include <geometry/geometry.hpp>
#include <geometry/distance.hpp>

namespace geometry
{

	//----------------------------------------------------------------------
	// Function     : point_on_linestring -> rename to alongLine?
	//----------------------------------------------------------------------
	// Purpose      : Calculates coordinates of a point along a given line
	//                on a specified distance
	// Parameters   : const L& : line,
	//                float position: position to calculate point
	//                P& point: point to calculate
	// Return       : true if point lies on line
	//----------------------------------------------------------------------
	// Author       : Barend, Geodan BV Amsterdam
	// Date         : spring 1996
	//----------------------------------------------------------------------
	template <typename P, typename L>
	bool point_on_linestring(const L& line, const double& position, P& point)
	{
		double current_distance = 0.0;
		if (line.size() < 2)
		{
			return false;
		}

		typename L::const_iterator vertex = line.begin();
		typename L::const_iterator previous = vertex++;

		while(vertex != line.end())
		{
			double dist = distance(*previous, *vertex);
			current_distance += dist;

			if (current_distance > position)
			{
				// It is not possible that dist == 0 here because otherwise
				// the current_distance > position would not become true (current_distance is increased by dist)
				double fraction = 1.0 - ((current_distance - position) / dist);

				// point i is too far, point i-1 to near, add fraction of
				// distance in each direction
				point.x ( previous->x() + (vertex->x() - previous->x()) * fraction);
				point.y ( previous->y() + (vertex->y() - previous->y()) * fraction);

				return true;
			}
			previous = vertex++;
		}

		// point at specified position does not lie on line
		return false;
	}

}


#endif // _GEOMETRY_POINT_ON_LINE_HPP

