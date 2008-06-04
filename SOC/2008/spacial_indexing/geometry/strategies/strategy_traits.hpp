// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_STRATEGY_TRAITS_HPP
#define _GEOMETRY_STRATEGY_TRAITS_HPP

#include <geometry/strategies/strategies_point_xy.hpp>
#include <geometry/strategies/strategies_point_ll.hpp>


namespace geometry
{

	namespace strategy
	{
		/*!
			\brief Indicate compiler/library user that strategy is not implemented.
			\details The strategy_traits class define strategies for point types or for point type
			combinations. If there is no implementation for that specific point type, or point type
			combination, the calculation cannot be done. To indicate this, this not_implemented
			class is used as a typedef stub.

		*/
		struct not_implemented {};
	}

	/*!
		\brief Base strategy class, defines not_implemented for all strategies.
		\details The strategy_traits class defines strategies for:
		- distance point to point
		- distance point to line
		- area calculation
		- centroid calculation
		- envelope calculation
		- within detection
		\note This is a base class, does NOT compile because doesn't define anything useful.
		Specializations should define the correct implementations classes.
		\note In case of compiler errors here, implement the correct specializations
	*/
	template <typename P1, typename P2 = P1>
	struct strategy_traits
	{
		typedef strategy::not_implemented point_distance;
		typedef strategy::not_implemented point_segment_distance;
		typedef strategy::not_implemented area;
		typedef strategy::not_implemented within;
		typedef strategy::not_implemented centroid;
		typedef strategy::not_implemented envelope;
	};


	/*!
		\brief Specialization for point_ll<degree/radian, T>
	*/
	template <dr_selector D1, typename T1, dr_selector D2, typename T2>
	struct strategy_traits<point_ll<D1, T1>, point_ll<D2, T2> >
	{
		typedef strategy::distance::haversine<point_ll<D1, T1>, point_ll<D2,T2> > point_distance;

		// The point-segment calculation also uses a point-point calculation.
		// For latlong, that encapsulated point-point calculation should always be defined in radian
		typedef strategy::distance::ll_point_segment<point_ll<D1, T1>, point_ll<D2, T2> > point_segment_distance;

		typedef strategy::area::by_spherical_excess<point_ll<D1, T1> > area;

		typedef strategy::not_implemented within;
		typedef strategy::not_implemented centroid;
		typedef strategy::envelope::grow_ll<point_ll<D1, T1>, point_ll<D2, T2> > envelope;

	};


	/*!
		\brief Specialization for point_xy<T>
	*/
	template <typename T1, typename T2>
	struct strategy_traits<point_xy<T1>, point_xy<T2> >
	{
		typedef strategy::distance::pythagoras<point_xy<T1>, point_xy<T2> > point_distance;
		typedef strategy::distance::xy_point_segment<point_xy<T1>, point_xy<T2>, point_distance> point_segment_distance;

		typedef strategy::area::by_triangles<point_xy<T1> > area;
		typedef strategy::within::winding<point_xy<T1>, point_xy<T2> > within;

		typedef strategy::centroid::bashein_detmer<point_xy<T1>,  point_xy<T2> > centroid;

		typedef strategy::envelope::grow_xy<point_xy<T1>,  point_xy<T2> > envelope;

	};


} // namespace geometry


#endif // _GEOMETRY_STRATEGY_TRAITS_HPP
