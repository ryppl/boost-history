// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <list>
#include <deque>
#include <algorithm> // for reverse, unique
#include <iterator>

#include <iostream>

#include <geometry/geometry.hpp>
#include <geometry/envelope.hpp>
#include <geometry/area.hpp>
#include <geometry/foreach.hpp>
#include <geometry/centroid.hpp>
//#include <geometry/labelinfo.hpp>
#include <geometry/streamwkt.hpp>
#include <geometry/correct.hpp>
#include <geometry/within.hpp>

#include <geometry/simplify.hpp>



std::string boolstr(bool v)
{
	return v ? "true" : "false";
}

int main(void)
{
	// Make a shortcut for a point
	typedef geometry::point_xy<float> P;

	// Define a polygon and fill the outer ring.
	// In most cases you will read it from a file or database

	geometry::polygon<P, std::vector, std::list> poly;
	{
		const double coor[][2] = {
			{2.0, 1.3}, {2.4, 1.7}, {2.8, 1.8}, {3.4, 1.2}, {3.7, 1.6},
			{3.4, 2.0}, {4.1, 3.0}, {5.3, 2.6}, {5.4, 1.2}, {4.9, 0.8}, {2.9, 0.7},
			{2.0, 1.3} // closing point is opening point
			};
		const int n = sizeof(coor) / sizeof(coor[0]);
		poly.outer().reserve(n);
		for (int i = 0; i < n; i++)
		{
			poly.outer().push_back(P(coor[i][0], coor[i][1]));
		}
	}

	// Polygons should be closed, and directed clockwise. If you're not sure if that is the case,
	// call the correct algorithm
	//geometry::correct(poly);

	// Polygons can be streamed as Well Known Text (OGC WKT)
	std::cout << poly << std::endl;

	// As with lines, bounding box of polygons can be calculated
	geometry::box<geometry::point_xy<float> > b;
	geometry::envelope(poly, b);
	std::cout << b.min() << "-" << b.max() << std::endl;


	// The area of the polygon can be calulated
	std::cout << "area: " << geometry::area(poly) << std::endl;

	// And the centroid, which is the center of gravity
	P centroid;
	geometry::centroid(poly, centroid);
	std::cout << "centroid: " << centroid << std::endl;

	// Actually in most cases you don't want the centroid, which is only defined for polygons.
	// You want a nice labelpoint instead. Call labelpoint in those cases, which is defined
	// for all geometries
	/* not yet in preview, syntax might change
	std::cout << "labelpoints: ";
	geometry::label_info<0>(poly, , std::ostream_iterator<P>(std::cout, " "));
	std::cout << std::endl;
	*/

	// The number of points have to called per ring separately
	std::cout << "number of points in outer ring: " << poly.outer().size() << std::endl;

	// Polygons can have one or more inner rings, also called holes, donuts, islands, interior rings.
	// Let's add one
	{
		poly.inners().resize(1);
		geometry::linear_ring<P>& inner = poly.inners().back();

		const double coor[][2] = { {4.0, 2.0}, {4.2, 1.4}, {4.8, 1.9}, {4.4, 2.2}, {4.0, 2.0} };
		int n = sizeof(coor) / sizeof(coor[0]);

		inner.reserve(n);
		for (int i = 0; i < n; i++)
		{
			inner.push_back(P(coor[i][0], coor[i][1]));
		}
	}

	geometry::correct(poly);

	std::cout << "with inner ring:" << poly << std::endl;
	// The area of the polygon is changed of course
	std::cout << "new area of polygon: " << geometry::area(poly) << std::endl;
	geometry::centroid(poly, centroid);
	std::cout << "new centroid: " << centroid << std::endl;

	// You can test whether points are within a polygon
	std::cout << "point in polygon:"
		<< " p1: "  << boolstr(geometry::within(P(3.0, 2.0), poly))
		<< " p2: "  << boolstr(geometry::within(P(3.7, 2.0), poly))
		<< " p3: "  << boolstr(geometry::within(P(4.4, 2.0), poly))
		<< std::endl;

	// You can call for_each or for_each_segment on polygons to, this will visit all points / segments
	// in outer ring and inner rings
	//geometry::for_each(poly, f);

	// As with linestrings and points, you can derive from polygon to add, for example,
	// fill color and stroke color. Or SRID (spatial reference ID). Or Z-value. Or a property map.
	// We don't show this here.

	// You can clip the polygon using a bounding box

	// If you really want:
	//   You don't have to use a vector, you can define a polygon with a deque or list
	//   You can specify the container for the points and for the inner rings independantly

	geometry::polygon<P, std::vector, std::list> poly2;
	poly2.outer().push_back(P(2.8, 1.9));
	poly2.outer().push_back(P(2.9, 2.4));
	poly2.outer().push_back(P(3.3, 2.2));
	poly2.outer().push_back(P(3.2, 1.8));
	poly2.outer().push_back(P(2.8, 1.9));

	return 0;
}
