// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_MULTI_AREA_HPP
#define _GEOMETRY_MULTI_AREA_HPP

#include <vector>

#include <geometry/area.hpp>

namespace geometry
{
	namespace impl
	{
		namespace area
		{

			template<typename MY, typename S>
			inline double area_multi_polygon(const MY& my, const S& strategy)
			{
				double a = 0.0;
				for (typename MY::const_iterator i = my.begin(); i != my.end(); i++)
				{
					a += impl::area::area_polygon(*i, strategy);
				}
				return a;
			}
		}
	}



	template<typename Y,
			template<typename,typename> class V, template<typename> class A, typename S>
	inline double area(const multi_polygon<Y, V, A>& m, const S& strategy)
	{
		return impl::area::area_multi_polygon(m, strategy);
	}

	template<typename Y,
			template<typename,typename> class V, template<typename> class A>
	inline double area(const multi_polygon<Y, V, A>& m)
	{
		return impl::area::area_multi_polygon(m, strategy_traits<typename Y::point_type>::area());
	}


} // namespace geometry


#endif // _GEOMETRY_MULTI_AREA_HPP
