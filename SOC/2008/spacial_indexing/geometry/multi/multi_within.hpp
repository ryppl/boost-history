// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_MULTI_WITHIN_HPP
#define _GEOMETRY_MULTI_WITHIN_HPP

#include <vector>

#include <geometry/within.hpp>

namespace geometry
{
	namespace impl
	{
		namespace within
		{


			template<typename P, typename O, typename S>
			inline bool point_in_multi_polygon(const P& p, const O& m, const S& strategy)
			{
				for (typename O::const_iterator i = m.begin(); i != m.end(); i++)
				{
					// Point within a multi-polygon: true if within one of the polygons
					if (point_in_polygon(p, *i, strategy))
					{
						return true;
					}
				}
				return false;
			}


			template<typename O, typename C>
			inline bool multi_polygon_in_circle(const O& m, const C& c)
			{
				for (typename O::const_iterator i = m.begin(); i != m.end(); i++)
				{
					if (! polygon_in_circle(*i, c))
					{
						return false;
					}
				}
				return true;
			}



		} // namespace within
	} // namespace impl


	template<typename C, typename T,
			typename Y,
			template<typename,typename> class V,
			template<typename> class A>
	inline bool within(const multi_polygon<Y, V, A> m, const circle<C, T>& c)
	{
		return impl::within::multi_polygon_in_circle(m, c);
	}




	template<typename P, typename Y,
			template<typename,typename> class V, template<typename> class A>
	inline bool within(const P& p, const multi_polygon<Y, V, A>& m)
	{
		return impl::within::point_in_multi_polygon(p, m,
					strategy_traits<P, typename Y::point_type>::within());
	}

	template<typename P, typename Y,
			template<typename,typename> class V, template<typename> class A, typename S>
	inline bool within(const P& p, const multi_polygon<Y, V, A>& m, const S& strategy)
	{
		return impl::within::point_in_multi_polygon(p, m, strategy);
	}


} // namespace geometry


#endif // _GEOMETRY_MULTI_WITHIN_HPP
