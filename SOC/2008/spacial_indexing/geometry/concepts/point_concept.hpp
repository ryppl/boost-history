// Geometry Library Point concept
//
// Copyright Bruno Lalande 2008
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef _GEOMETRY_POINT_CONCEPT_HPP
#define _GEOMETRY_POINT_CONCEPT_HPP


#include <boost/concept_check.hpp>
#include <geometry/concepts/point_traits.hpp>


namespace geometry
{
	template <typename X>
	struct Point
	{
		typedef typename point_traits<X>::coordinate_type ctype;
		enum { ccount = point_traits<X>::coordinate_count };


		template <typename P, int I, int Count>
		struct dimension_checker
		{
			static void check()
			{
				const P* point;
				ctype coord = point_traits<const X>::template get<I>(*point);
				P* point2;
				point_traits<X>::template get<I>(*point2) = coord;

				dimension_checker<P, I+1, Count>::check();
			}
		};

		template <typename P, int Count>
		struct dimension_checker<P, Count, Count>
		{
			static void check() {}
		};


		BOOST_CONCEPT_USAGE(Point)
		{
			dimension_checker<X, 0, ccount>::check();
		}
	};
}


#endif
