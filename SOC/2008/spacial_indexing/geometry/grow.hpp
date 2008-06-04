// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_GROW_HPP
#define _GEOMETRY_GROW_HPP

// Grow functions for boxes

#include <geometry/geometry.hpp>
#include <geometry/arithmetic.hpp>
#include <geometry/box.hpp>

namespace geometry
{
	namespace impl
	{
		namespace grow
		{

			template <typename PS, typename PD, int I, int N>
			struct check_minmax
			{
				static void check(const PS& source, PD& min, PD& max)
				{
					typedef typename point_traits<PS>::coordinate_type T;
					const T& coordinate = get<I>(source);

					if (coordinate < get<I>(min))
					{
						get<I>(min) = coordinate;
					}
					if (coordinate > get<I>(max))
					{
						get<I>(max) = coordinate;
					}

					check_minmax<PS, PD, I+1, N>::check(source, min, max);
				}
			};

			template <typename PS, typename PD, int N>
			struct check_minmax<PS, PD, N, N>
			{
				static void check(const PS& source, PD& min, PD& max)
				{}
			};

			// Changes an box b such that it also contains point p
			template<typename B, typename P>
			inline void grow_p(B& b, const P& p)
			{
				check_minmax<typename B::point_type, P, 0, 
						point_traits<P>::coordinate_count>::check(p, b.min(), b.max());			
			}

			// Changes an box such that box is also contained by the box
			template<typename B1, typename B2>
			inline void grow_b(B1& b, const B2& other)
			{
				grow_p(b, other.min());
				grow_p(b, other.max());
			}

			// Extends an box with the same amount in all directions
			template<typename B, typename T>
			inline void grow_t(B& b, const T& value)
			{
				add_value(b.min(), -value);
				add_value(b.max(), +value);
			}

		} // namespace grow
	} // namespace impl

	// Changes an box such that point p is also contained by the box
	template<typename P>
	inline void grow(box<P>& b, const P& p)
	{
		impl::grow::grow_p(b, p);
	}

	// Changes an box such that box is also contained by the box
	template<typename P>
	inline void grow(box<P>& b, const box<P>& other)
	{
		impl::grow::grow_b(b, other);
	}

	// Extends an box with the same amout in all directions
	template<typename P, typename T>
	inline void grow(box<P>& b, const T& value)
	{
		impl::grow::grow_t(b, value);
	}

};

#endif // _GEOMETRY_GROW_HPP
