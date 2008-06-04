// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <geometry/geometry.hpp>
#include <geometry/distance.hpp>
#include <geometry/streamwkt.hpp>

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>


int main()
{
	geometry::point_xy<double> world(1, 2);
	std::cout << "Hello " << world << std::endl;

	// There are some constructors. Default constructor does NOTHING.
	// Other constructors can put zero or infinity
	geometry::point_xy<double> p1(geometry::init_zero);
	geometry::point_xy<double> p2(geometry::init_min_infinite);

	geometry::point_xy<char> pch(geometry::init_min_infinite);

	// Points may be "double", "float", "int", "char", etc. etc.
	geometry::point_xy<float> p3(3, 2);
	geometry::point_xy<char> p4(100, 108);

	// Define two points...
	geometry::point_xy<double> a(1, 2);
	geometry::point_xy<double> b(5, 6);

	// The distance between them can be calculated
	std::cout << "distance " << a << " to " << b << " is " << geometry::distance(a,b) << std::endl;

	// Points can be compared to each other
	std::cout << a << (a == b ? " == " : " <> ") << b << std::endl;
	std::cout << a << (a < b ? " < " : " >= ") << b << std::endl;

	// Because of comparison, containers with points are sortable
	std::vector<geometry::point_xy<double> > v;
	v.push_back(b);
	v.push_back(a);

	std::copy(v.begin(), v.end(), std::ostream_iterator<geometry::point_xy<double> >(std::cout, " "));
	std::cout << std::endl;

	std::sort(v.begin(), v.end());

	std::copy(v.begin(), v.end(), std::ostream_iterator<geometry::point_xy<double> >(std::cout, " "));
	std::cout << std::endl;

	// Points can be derived or have own implementations, shown in "other_point_example".
	// Otherwise continue with "linestring_example"

	return 0;
}
