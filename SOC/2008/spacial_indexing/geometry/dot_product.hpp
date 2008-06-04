// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef _GEOMETRY_DOT_PRODUCT_HPP
#define _GEOMETRY_DOT_PRODUCT_HPP


#include <geometry/concepts/point_traits.hpp>


namespace geometry
{
	namespace impl
	{
		template <int I, int N>
		struct dot_product_maker
		{
			template <typename P1, typename P2>
			static typename point_traits<P1>::coordinate_type
			run(const P1& p1, const P2& p2)
			{
				return get<I>(p1)*get<I>(p2)
				     + dot_product_maker<I+1, N>::run(p1, p2);
			}
		};

		template <int N>
		struct dot_product_maker<N, N>
		{
			template <typename P1, typename P2>
			static typename point_traits<P1>::coordinate_type
			run(const P1& p1, const P2& p2)
			{
				return get<N>(p1)*get<N>(p2);
			}
		};

	} // namespace impl


	template <typename P1, typename P2>
	typename point_traits<P1>::coordinate_type
	dot_product(const P1& p1, const P2& p2)
	{
		return impl::dot_product_maker<0, point_traits<P1>::coordinate_count - 1>::run(p1, p2);
	}

} // namespace geometry


#endif
