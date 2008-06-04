// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_DISTANCE_HPP
#define _GEOMETRY_DISTANCE_HPP

#include <cmath>

#include <geometry/geometry.hpp>
#include <geometry/segment.hpp>

#include <geometry/strategies/strategy_traits.hpp>

#include <geometry/util/promotion_traits.hpp>

/*!
\defgroup distance distance: distance calculation algorithms
The distance algorithm returns the distance between two geometries. The distance is always returned
as a double type, regardless of the input type, because the distance between integer coordinates is
a square root and might be a double.
\note
All primary distance functions return a std::pair<double, bool> with (distance, squared)
if squared=true, it is the squared distance, otherwise it is the distance.
This is for efficiency, often distance is used for comparisons only and then squared is OK.
However, some algorithms such as great circle do NOT use sqrt in their formula and then 
it is not necessary to first calculate the sqr and lateron take the sqrt
*/

namespace geometry
{


	namespace impl
	{
		namespace distance
		{
			template <typename P1, typename P2, typename S>
			inline distance_result point_to_point(const P1& p1, const P2& p2, const S& strategy)
			{
				return strategy(p1, p2);
			}

			template<typename P, typename L, typename S>
			inline distance_result point_to_linestring(const P& p, const L& ln, const S& strategy)
			{
				if (ln.size() <= 0)
				{
					return distance_result(0, false);
				}

				// line of one point: return point square_distance
				typename L::const_iterator it = ln.begin();
				if (ln.size() == 1)
				{
					typename S::distance_strategy_type pp;
					return pp(p, *it);
				}

				typedef const_segment<typename L::point_type> CS;

				// start with first segment distance
				typename L::const_iterator prev = it++;

				typename strategy_traits<P>::point_segment_distance f2;
				distance_result d = f2(p, CS(*prev, *it));

				// check if other segments are closer
				prev = it++;
				while(it != ln.end())
				{
					distance_result ds = f2(p, CS(*prev, *it));
					if (ds.first <= std::numeric_limits<double>::epsilon())
					{
						return distance_result(0, false);
					}
					else if (ds.first < d.first)
					{
						d = ds;
					}
					prev = it++;
				}
				return d;
			} 

		} // namespace distance

	} // namespace impl



	/*! 
		\brief Calculate distance between two points
		\ingroup distance
		\details This version of distance calculates the distance between two points, using the specified strategy
		\param p1 first point
		\param p2 second point
		\param strategy strategy to calculate distance between two points
		\return the distance
		\par Example:
		Example showing distance calculation of two lat long points, using the accurate Vincenty approximation
		\dontinclude doxygen_examples.cpp
		\skip example_distance_point_point_strategy
		\line {
		\until }
	 */
	template <typename P1, typename P2, typename S>
	inline double distance(const P1& p1, const P2& p2, const S& strategy)
	{
		distance_result result = impl::distance::point_to_point(p1, p2, strategy);
		return result;
	}

	/*! 
		\brief Calculate distance between two points
		\ingroup distance
		\details This version of distance calculates the distance between two points, using the default distance-calculation-strategy
		\param p1 first point
		\param p2 second point
		\return the distance
		\note The two point may be of different types, if there is a strategy_traits specialization for this type combination
		\par Example:
		Example showing distance calculation of two points, in xy and in latlong coordinates
		\dontinclude doxygen_examples.cpp
		\skip example_distance_point_point
		\line {
		\until }
	 */
	template <typename P1, typename P2>
	inline double distance(const P1& p1, const P2& p2)
	{
		return distance(p1, p2, typename strategy_traits<P1, P2>::point_distance());
	}


	/*! 
		\brief Calculate distance between a point and a linestring
		\ingroup distance
		\details This version of distance calculates the distance between a point and a linestring, using the default strategy
		\param p point
		\param ln linestring
		\return the distance
		\note The point might be of another type the points in the linestring, if there is a strategy_traits 
		specialization for this type combination
	 */
	template<typename P1, typename P2,
				template<typename,typename> class V, template<typename> class A>
	inline double distance(const P1& p, const linestring<P2, V, A>& ln)
	{
		return distance(p, ln, 
			typename strategy_traits<P1, P2>::point_segment_distance());
	}

	/*! 
		\brief Calculate distance between a point and a linestring
		\ingroup distance
		\details This version of distance calculates the distance between a point and a linestring, using the specified strategy
		\param p point
		\param ln linestring
		\param strategy strategy to calculate distance of point to segment
		\return the distance
		\note The point might be of another type the points in the linestring, if the specified strategy allows this
		\note The strategy might implement an enclosed point-point distance strategy
	 */
	template<typename P1, typename P2,
				template<typename,typename> class V, template<typename> class A, typename S>
	inline double distance(const P1& p, const linestring<P2, V, A>& ln, const S& strategy)
	{
		distance_result result = impl::distance::point_to_linestring(p, ln, strategy);
		return result;
	}


} // namespace geometry


#endif // _GEOMETRY_DISTANCE_HPP
