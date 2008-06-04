// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_OVERLAPS_HPP
#define _GEOMETRY_OVERLAPS_HPP

#include <geometry/geometry.hpp>
#include <geometry/box.hpp>

namespace geometry
{
	template <typename P>
	bool overlaps(const box<P>& r1, const box<P>& r2)
	{
		return !(
				r1.max().x() <= r2.min().x() ||
				r1.min().x() >= r2.max().x() ||
				r1.max().y() <= r2.min().y() ||
				r1.min().y() >= r2.max().y()
				);

	}


} // namespace geometry


#endif // _GEOMETRY_OVERLAPS_HPP
