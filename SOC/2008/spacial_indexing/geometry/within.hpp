// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_WITHIN_HPP
#define _GEOMETRY_WITHIN_HPP

#include <boost/static_assert.hpp>

#include <geometry/geometry.hpp>
#include <geometry/segment.hpp>
#include <geometry/circle.hpp>
#include <geometry/box.hpp>
#include <geometry/distance.hpp>
#include <geometry/util/side.hpp>

#include <geometry/loop.hpp>

#include <geometry/strategies/strategy_traits.hpp>
#include <geometry/strategies/strategies_point_xy.hpp>


/*!
\defgroup within within: containment algorithms such as point in polygon
The within algorithm checks if one geometry is located completely within another geometry.
A well-known example is point-in-polygon, returning true if a point falls within a polygon.
Many point-in-polygon algorithms ignore boundary cases (point is situated on boundary of polygon),
however the within_point_in_polygon method should return true if a point is completely within the
boundary of the polygon, and not on the boundary.
*/


namespace geometry
{
	/*!
		\ingroup impl
	 */
	namespace impl
	{
		namespace within
		{

			// Within, should return true if one geometry falls completely within another geometry
			// Geometries can lie in something with an area, so in an box,circle,linear_ring,polygon

			//-------------------------------------------------------------------------------------------------------
			// Implementation for boxes. Supports boxes in 2 or 3 dimensions, in Euclidian system
			// Todo: implement as strategy
			//-------------------------------------------------------------------------------------------------------
			template <typename P, typename B, int I, int N>
			struct point_in_box
			{
				static bool inside(const P& p, const B& b)
				{
					if (get<I>(p) < get<I>(b.min()) || get<I>(p) > get<I>(b.max()))
					{
						return false;
					}

					return point_in_box<P, B, I+1, N>::inside(p, b);
				}
			};

			template <typename P, typename B, int N>
			struct point_in_box<P, B, N, N>
			{
				static bool inside(const P& p, const B& b)
				{
					return true;
				}
			};


			//-------------------------------------------------------------------------------------------------------
			// Implementation for circles. Supports circles or spheres, in 2 or 3 dimensions, in Euclidian system
			// Circle center might be of other point-type as geometry
			// Todo: implement as strategy
			//-------------------------------------------------------------------------------------------------------
			template<typename P, typename C>
			inline bool point_in_circle(const P& p, C& c)
			{
				strategy::distance::pythagoras<P, typename C::point_type> s;
				distance_result r = impl::distance::point_to_point(p, c.center(), s);
				return r.first < c.radius() * c.radius();
			}


			template<typename B, typename C>
			inline bool box_in_circle(const B& b, const C& c)
			{
				typedef typename B::point_type P;

				// Currently only implemented for 2d points
				BOOST_STATIC_ASSERT(point_traits<P>::coordinate_count == 2);

				// Box: all four points must lie within circle

				// Check points lower-left and upper-right
				if (point_in_circle(b.min(), c) && point_in_circle(b.max(), c))
				{
					// Check points lower-right and upper-left
					P p1;
					P p2;
					get<0>(p1) = get<0>(b.min());
					get<1>(p1) = get<1>(b.max());
					get<0>(p2) = get<0>(b.max());
					get<1>(p2) = get<1>(b.min());
					return point_in_circle(p1, c) && point_in_circle(p2, c);
				}
				return false;
			}


			// Generic "vector of points - in - circle", true if all points within circle
			template<typename V, typename C>
			inline bool container_in_circle(const V& v, const C& c)
			{
				for (typename V::const_iterator i = v.begin(); i != v.end(); i++)
				{
					if (! point_in_circle(*i, c))
					{
						return false;
					}
				}
				return true;
			}

			template<typename Y, typename C>
			inline bool polygon_in_circle(const Y& poly, const C& c)
			{
				return container_in_circle(poly.outer(), c);
			}




			template<typename P, typename R, typename S>
			inline bool point_in_ring(const P& p, const R& r, const S& strategy)
			{
				if (r.size() < 4)
				{
					return false;
				}

				typename S::state_type state(p);
				if (loop(r, strategy, state))
				{
					return state.within();
				}
				return false;
			}

			// Polygon: in outer linear_ring, and if so, not within inner ring(s)
			template<typename P, typename Y, typename S>
			inline bool point_in_polygon(const P& p, const Y& poly, const S& strategy)
			{
				if (point_in_ring(p, poly.outer(), strategy))
				{
					for (typename Y::inner_container_type::const_iterator i = poly.inners().begin();
									i != poly.inners().end(); i++)
					{
						if (point_in_ring(p, *i, strategy))
						{
							return false;
						}
					}
					return true;
				}
				return false;
			}


		} // namespace within
	} // namespace impl




	template<typename P1, typename P2>
	inline bool within(const P1& p, const box<P2>& b)
	{
		BOOST_STATIC_ASSERT(point_traits<P1>::coordinate_count == point_traits<P2>::coordinate_count);
		return impl::within::point_in_box<P1, box<P2>, 
					0, point_traits<P1>::coordinate_count>::inside(p, b);
	}

	template<typename P, typename C, typename T>
	inline bool within(const P& p, const circle<C, T>& c)
	{
		return impl::within::point_in_circle(p, c);
	}

	template<typename P, typename C, typename T>
	inline bool within(const box<P>& b, const circle<C, T>& c)
	{
		return impl::within::box_in_circle(b, c);
	}

	template<typename C, typename T,
			typename P,
			template<typename,typename> class V,
			template<typename> class A>
	inline bool within(const linestring<P, V, A>& ln, const circle<C, T>& c)
	{
		return impl::within::container_in_circle(ln, c);
	}

	template<typename C, typename T,
			typename P,
			template<typename,typename> class V,
			template<typename> class A>
	inline bool within(const linear_ring<P, V, A>& r, const circle<C, T>& c)
	{
		return impl::within::container_in_circle(r, c);
	}

	template<typename C, typename T,
			typename P,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR>
	inline bool within(const polygon<P, VP, VR, AP, AR>& poly, const circle<C, T>& c)
	{
		return impl::within::polygon_in_circle(poly, c);
	}


	template<typename P1, typename P2,
			template<typename,typename> class V,
			template<typename> class A>
	inline bool within(const P1& p, const linear_ring<P2, V, A>& r)
	{
		return impl::within::point_in_ring(p, r);
	}

	/*! 
		\brief Examine if point is in polygon
		\ingroup within
		\details The function within, for point-polygon, returns the point lies completely within the polygon
		\param p point to examine
		\param poly polygon to examine
		\return true if point is completely contained within the polygon, else false
		\par Example:
		The within algorithm is used as following:
		\dontinclude doxygen_examples.cpp
		\skip example_within
		\line {
		\until }
		\note The strategy bound by the specialized strategy_traits class is used for within detection
	 */
	template<typename P1, typename P2,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR>
	inline bool within(const P1& p, const polygon<P2, VP, VR, AP, AR>& poly)
	{
		return impl::within::point_in_polygon(p, poly, 
			typename strategy_traits<P1,P2>::within());
	}

	/*! 
		\brief Examine if point is in polygon
		\ingroup within
		\details The function within, for point-polygon, returns the point lies within the polygon, using a specified strategy
		\param p point to examine
		\param poly polygon to examine
		\param strategy optional strategy to be used for within detection.
		       If not specified, the default within-detection strategy is taken.
		\return true if point is within the polygon, else false
		\par Example:
		Example showing how to use within using a strategy
		\dontinclude doxygen_examples.cpp
		\skip example_within_strategy
		\line {
		\until }
		\note The strategy bound by the specialized strategy_traits class is used for within detection
	 */
	template<typename P1, typename P2,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR,
			typename S>
	inline bool within(const P1& p, const polygon<P2, VP, VR, AP, AR>& poly, const S& strategy)
	{
		return impl::within::point_in_polygon(p, poly, strategy);
	}


} // namespace geometry


#endif // _GEOMETRY_WITHIN_HPP
