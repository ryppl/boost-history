// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_BOX_HPP
#define _GEOMETRY_BOX_HPP

#include <geometry/geometry.hpp>
#include <geometry/assign.hpp>

namespace geometry
{

	/*! 
		\brief Class box: two points describing a box
		\details Box is always described by a min() and a max() point. If another
		rectangle is used, use linear_ring or polygon.
		The point can be a point_xy or a point_ll point.
		\note Used for selections and for calculating the evnvelope. Not all algorithms
		are implemented for box
		\par Template parameters:
		- \a P point type of the minimum and maximum box
	 */

	template<typename P>
	class box : public geometry_traits<P>
	{
		public :

			inline box()
			{
				// Initialize to zero (?), we might change this to nothing, because
				// there is already another constructor which does this.
				assign_value(m_min, T()); 
				assign_value(m_max, T()); 
			}

			inline box(const P& min, const P& max)
				: m_min(min)
				, m_max(max)
			{}

			/* Removed because not coordinate neutral 
			and makes assumptions about constructor of P
			inline box(T& x1, T& y1, T& x2, T& y2)
				: m_min(x1, y1)
				, m_max(x2, y2)
			{}
			*/

			inline box(init option)
			{
				switch(option)
				{
					case init_inverse :
						assign_value(m_min, boost::numeric::bounds<T>::highest()); 
						assign_value(m_max, boost::numeric::bounds<T>::lowest()); 
						break;
					case init_zero :
						assign_value(m_min, T()); 
						assign_value(m_max, T()); 
						break;
					default :
						// no construction at all
						break;
				}
			}


			inline const P& min() const { return m_min; }
			inline const P& max() const { return m_max; }

			inline P& min() { return m_min; }
			inline P& max() { return m_max; }

		private :
			P m_min, m_max;
			typedef typename point_traits<P>::coordinate_type T;
	};


};

#endif // _GEOMETRY_BOX_HPP
