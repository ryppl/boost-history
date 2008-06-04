// Geometry Library Point traits
//
// Copyright Bruno Lalande 2008
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef _GEOMETRY_POINT_TRAITS_HPP
#define _GEOMETRY_POINT_TRAITS_HPP


namespace geometry
{
	// Fall-back definition, using the point class' nested properties

	template <typename P>
	struct point_traits
	{
		typedef typename P::coordinate_type coordinate_type;
		enum { coordinate_count = P::coordinate_count };

		template <int I>
		static coordinate_type& get(P& p)
		{ return p.template get<I>(); }
	};

	template <typename P>
	struct point_traits<const P>
	{
		typedef typename P::coordinate_type coordinate_type;
		enum { coordinate_count = P::coordinate_count };

		template <int I>
		static const coordinate_type& get(const P& p)
		{ return p.template get<I>(); }
	};


	// Specialization for arrays

	template <typename T, int N>
	struct point_traits<T[N]>
	{
		typedef T coordinate_type;
		enum { coordinate_count = N };

		template <int I>
		static T& get(T p[N])
		{ return p[I]; }
	};

	template <typename T, int N>
	struct point_traits<const T[N]>
	{
		typedef T coordinate_type;
		enum { coordinate_count = N };

		template <int I>
		static const T& get(const T p[N])
		{ return p[I]; }
	};


	// Global get<>() functions

	template <int I, typename P>
	static typename point_traits<P>::coordinate_type&
	get(P& p)
	{
		return point_traits<P>::template get<I>(p);
	}

	template <int I, typename P>
	static const typename point_traits<const P>::coordinate_type&
	get(const P& p)
	{
		return point_traits<const P>::template get<I>(p);
	}
}


#endif
