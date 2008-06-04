// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_CIRCLE_HPP
#define _GEOMETRY_CIRCLE_HPP

#include <geometry/geometry.hpp>

namespace geometry
{
	/*! 
		\brief Class circle: point with radius
		\details Currently only implemented for point_xy with a radius in the units
		of the coordinate system.
		\note Used for selections, for example polygon_in_circle. Currently not all
		algorithms are implemented for circles.
		\par Template parameters:
		- \a P point type of the circle center
		- \a T number type of the radius
	 */
	template <typename P, typename T>
	class circle : public geometry_traits<P>
	{
		public :
			typedef T radius_type;

			circle()
				: m_center(init_zero)
				, m_radius(0.0)
			{}

			circle(const P& center, const T& radius)
				: m_center(center)
				, m_radius(radius)
			{}

			circle(const typename point_traits<P>::coordinate_type& x,
					const typename point_traits<P>::coordinate_type& y,
					const T& radius)
				: m_center(x, y)
				, m_radius(radius)
			{}

			inline const P& center() const { return m_center; }
			inline const T radius() const { return m_radius; }

			inline void radius(const T& r) { m_radius = r; }
			inline P& center() { return m_center; }

		private :
			P m_center;
			T m_radius;
	};

}

#endif // _GEOMETRY_CIRCLE_HPP
