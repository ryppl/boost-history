// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_ASSIGN_HPP
#define _GEOMETRY_ASSIGN_HPP

#include <geometry/geometry.hpp>

/*!
\defgroup utility Utility: utilities (copy coordinates, assign coordinates in "neutral" way)
*/

namespace geometry
{
	namespace impl
	{
		namespace assign
		{
			template <typename P, size_t I, size_t N>
			struct assign_coordinate
			{
				static void assign(P& p, size_t i, const typename P::coordinate_type& v)
				{
					if (i == I)
					{
						p.template get<I>() = v;
					}
					else
					{
						assign_coordinate<P, I+1, N>::assign(p, i, v);
					}
				}
			};

			template <typename P, size_t N>
			struct assign_coordinate<P, N, N>
			{
				static void assign(P& p, size_t i, const typename P::coordinate_type& v)
				{}
			};



			template <typename C>
			struct assign_operation
			{
				assign_operation(C value) : m_value(value) {}

				template <typename P, int I>
				void operator()(P& point) const
				{ 
					get<I>(point) = m_value; 
				}

				private : 
					C m_value;
			};


		} // namespace assign

	} // namespace impl



	/*!
		\brief Assigns a coordinate
		\ingroup utility
		\details
		\param p Point
		\param i index
		\param v Value which is assigned to coordinate i of point p
	 */
	template <typename P>
	void assign_coordinate(P& p, size_t i, const typename P::coordinate_type& v)
	{
		impl::assign::assign_coordinate<P, 0, P::coordinate_count>::assign(p, i, v);
	}

	/*!
		\brief Assigns all coordinates of a specific point to a value
		\ingroup utility
		\details
		\param p Point
		\param value Value which is assigned to all coordinates of point p
	 */
	template <typename P>
	void assign_value(P& p, const typename point_traits<P>::coordinate_type& value)
	{ 
		for_each_coordinate(p, impl::assign::assign_operation<typename point_traits<P>::coordinate_type>(value)); 
	}



} // namespace geometry


#endif // _GEOMETRY_ASSIGN_HPP
