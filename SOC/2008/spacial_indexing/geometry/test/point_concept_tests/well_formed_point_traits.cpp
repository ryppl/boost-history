// Geometry Library Point concept test file
//
// Copyright Bruno Lalande 2008
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include "function_requiring_a_point.hpp"


struct point
{
	point(): x(), y() {}
	float x, y;
};


template <int I> struct accessor;

template <>
struct accessor<0>
{
	static float& get(point& p) { return p.x; }
	static const float& get(const point& p) { return p.x; }
};

template <>
struct accessor<1>
{
	static float& get(point& p) { return p.y; }
	static const float& get(const point& p) { return p.y; }
};


namespace geometry
{
	template <>
	struct point_traits<point>
	{
		typedef float coordinate_type;
		enum { coordinate_count = 2 };

		template <int I>
		static float& get(point& p)
		{ return accessor<I>::get(p); }
	};

	template <>
	struct point_traits<const point>
	{
		typedef float coordinate_type;
		enum { coordinate_count = 2 };

		template <int I>
		static const float& get(const point& p)
		{ return accessor<I>::get(p); }
	};
}


int main()
{
	point p1;
	const point p2;
	geometry::function_requiring_a_point(p1, p2);
}
