// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_RETURN_TYPES_HPP
#define _GEOMETRY_RETURN_TYPES_HPP

#include <utility>

namespace geometry
{
	/*!
		\brief Encapsulate the results of distance calculation
		\details Distance calculation for xy points or xyz points is done by taking the square
		root. However, for distance comparison drawing the square root is not necessary.
		Therefore the distance strategies are allowed to return the squares of the distance.
		This structure contains the distance, and a boolean to indicate if it is squared.
		It has an automatic conversion to a double value, which does the square root if necessary.
	*/
	//template <typename T>
	struct distance_result : public std::pair<double, bool>
	{
		public :
			/// Constructor with a value and a boolean flag
			distance_result(double v, bool b)
				: std::pair<double, bool> (v, b)
			{}

			/// Automatic conversion to double, taking squareroot if necessary
			inline operator double() const
			{
				return second ? sqrt(first) : first;
			}
	};


} // namespace geometry


#endif // _GEOMETRY_RETURN_TYPES_HPP
