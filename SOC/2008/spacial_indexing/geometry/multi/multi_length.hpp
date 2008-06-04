// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_MULTI_LENGTH_HPP
#define _GEOMETRY_MULTI_LENGTH_HPP

#include <vector>

#include <geometry/length.hpp>

namespace geometry
{


	// Length of a multi-linestring
	template<typename ML>
	inline double length_multi_linestring(const ML& ml)
	{
		double sum = 0;
		for (typename ML::const_iterator i = ml.begin(); i != ml.end(); i++)
		{
			sum  += length_linestring(*i);
		}
		return sum;
	}


} // namespace geometry


#endif // _GEOMETRY_MULTI_LENGTH_HPP
