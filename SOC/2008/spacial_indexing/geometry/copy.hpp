// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_COPY_HPP
#define _GEOMETRY_COPY_HPP

#include <geometry/geometry.hpp>

/*!
\defgroup copy copy: copy utility
The copy utility method copies coordinate values
*/

namespace geometry
{
	namespace impl
	{
		namespace copy
		{
			template <typename PS, typename PD, int I, int N>
			struct copy_coordinates
			{
				static void copy(const PS& source, PD& dest)
				{
					// Todo: use boost-conversion-tool for the cast
					get<I>(dest) = (typename point_traits<PD>::coordinate_type) get<I>(source);
					copy_coordinates<PS, PD, I+1, N>::copy(source, dest);
				}
			};

			template <typename PS, typename PD, int N>
			struct copy_coordinates<PS, PD, N, N>
			{
				static void copy(const PS& source, PD& dest)
				{}
			};

		} // namespace copy

	} // namespace impl


	/*!
		\brief Copies coordinates from source to destination point
		\ingroup copy
		\details The function copy_coordinates copies coordinates from one point to another point.
		Source point and destination point might be of different types.
		\param source Source point
		\param dest Destination point
		\note If destination type differs from source type, they must have the same coordinate count
	 */
	template <typename PS, typename PD>
	void copy_coordinates(const PS& source, PD& dest)
	{
		BOOST_STATIC_ASSERT(point_traits<PD>::coordinate_count == point_traits<PS>::coordinate_count);
		impl::copy::copy_coordinates<PS, PD, 0, point_traits<PS>::coordinate_count>::copy(source, dest);
	}


} // namespace geometry


#endif // _GEOMETRY_COPY_HPP
