// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_MULTI_CORRECT_HPP
#define _GEOMETRY_MULTI_CORRECT_HPP

#include <vector>

#include <geometry/correct.hpp>

namespace geometry
{

	namespace impl
	{
		namespace correct
		{
			// correct a multi-polygon
			template <typename O>
			inline void correct_multi_polygon(O& o)
			{
				for (typename O::iterator it = o.begin(); it != o.end(); it++)
				{
					correct_polygon(*it);
				}
			}
		}
	}


} // namespace geometry


#endif // _GEOMETRY_MULTI_CORRECT_HPP
