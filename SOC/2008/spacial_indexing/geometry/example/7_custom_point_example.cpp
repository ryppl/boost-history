// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <iostream>
#include <boost/tuple/tuple.hpp>
#include <geometry/distance.hpp>
#include <geometry/aswkt.hpp>


struct my_3d_point: boost::tuple<float, float, float>
{
	typedef float coordinate_type;
	enum { coordinate_count = 3 };

	my_3d_point(float x, float y, float z)
	{
		get<0>() = x;
		get<1>() = y;
		get<2>() = z;
	}

	// Because the geometry::point concept shares the "get" methods with boost::tuple,
	// no more methods are needed.
};


namespace geometry {

template <>
struct strategy_traits<my_3d_point, my_3d_point>
{
	typedef strategy::distance::pythagoras<my_3d_point, my_3d_point> point_distance;
	typedef strategy::not_implemented point_segment_distance;
	typedef strategy::not_implemented area;
	typedef strategy::not_implemented within;
	typedef strategy::not_implemented centroid;
	typedef strategy::not_implemented envelope;
};

template <>
struct wkt_traits<my_3d_point>
{
	typedef impl::wkt::stream_point<my_3d_point> stream_type;
};

} // namespace geometry

template <typename CH, typename TR>
inline std::basic_ostream<CH,TR>& operator<<(std::basic_ostream<CH,TR> &os, const my_3d_point &p)
{
	os << geometry::as_wkt<my_3d_point>(p);
	return os;
}


int main()
{
	// Define 2 instances of our custom 3D point
	my_3d_point p1(11, -22, 33);
	my_3d_point p2(-111, 222, -333);

	// The distance between them can be calculated the same way as for the
	// point_xy provided with the library
	std::cout << "distance " << p1 << " to " << p2 << " is " << geometry::distance(p1,p2) << std::endl;

	return 0;
}
