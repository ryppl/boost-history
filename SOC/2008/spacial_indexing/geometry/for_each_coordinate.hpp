// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef _GEOMETRY_FOR_EACH_COORDINATE_HPP
#define _GEOMETRY_FOR_EACH_COORDINATE_HPP

#include "boost/config.hpp"

#include <geometry/concepts/point_traits.hpp>


namespace geometry
{
	namespace impl
	{
		template <typename P, int I, int N>
		struct coordinates_scanner
		{
			template <typename F>
			static void apply(P& point, F function)
			{
#if defined(BOOST_MSVC)
				function.operator()<P, I>(point);
#else
				function.template operator()<P, I>(point);
#endif
				coordinates_scanner<P, I+1, N>::apply(point, function);
			}
		};

		template <typename P, int N>
		struct coordinates_scanner<P, N, N>
		{
			template <typename F>
			static void apply(P&, F)
			{}
		};

	} // namespace impl


	template <typename P, typename F>
	void for_each_coordinate(P& point, F function)
	{
		impl::coordinates_scanner<
			P, 0, point_traits<P>::coordinate_count
		>::apply(point, function);
	}

} // namespace geometry


#endif
