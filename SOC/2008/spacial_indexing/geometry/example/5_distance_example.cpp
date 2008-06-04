// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


/***
 This sample demonstrates the use of latlong-points, xy-points, calculate distances between
 latlong points using different formulas,
 calculate distance between points using pythagoras
 ***/


#include <geometry/geometry.hpp>
#include <geometry/latlong.hpp>
#include <geometry/distance.hpp>
#include <geometry/streamwkt.hpp>
#include <geometry/length.hpp>

#ifdef use_projection
#include <projection/rd.hpp>
#endif

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>




int main()
{
	using namespace geometry;

	// ------------------------------------------------------------------------------------------
	// Construction and conversion
	// ------------------------------------------------------------------------------------------
	typedef point_ll<degree, double> lola;

	lola a;
	// Amsterdam 52°22'23"N 4°53'32"E
	a.lat(dms<north>(52, 22, 23));
	a.lon(dms<east>(4, 53, 32));

	// Rotterdam 51°55'51"N 4°28'45"E
	lola r(latitude<>(dms<north>(51, 55, 51)),
		longitude<>(dms<east>(4, 28, 45)));

	// The hague: 52° 4' 48" N, 4° 18' 0" E
	lola h(longitude<>(dms<east>(4, 18, 0)),
		latitude<>(dms<north>(52, 4, 48)));

	// Define same in Dutch coordinate-system (in meters)
	typedef point_xy<double> XY;
#ifdef use_projection
	projection::rd<point_xy<double>, point_ll<radian, double> > proj;
	XY a_rd = proj.project(a.convert<radian>());
	XY r_rd = proj.project(r.convert<radian>());
#else
	XY a_rd(121267, 487245);
	XY r_rd(92526.2, 438324);
	XY h_rd(80454.2, 455086);
#endif

	typedef point_ll<radian, double> lolar;
	lolar a_rad = a.convert<radian>(), r_rad = r.convert<radian>();

	// ------------------------------------------------------------------------------------------
	// Distances
	// ------------------------------------------------------------------------------------------

	std::cout << "Distance Amsterdam-Rotterdam: " << std::endl;
	std::cout << "haversine:              " << 0.001 * distance(a, r) << " km" << std::endl;
	std::cout << "haversine rad:          " << 0.001 * distance(a_rad, r_rad) << " km" << std::endl;
	std::cout << "haversine other radius: " << distance(a, r, strategy::distance::haversine<lola>(6371.0) ) << " km" << std::endl;
	std::cout << "andoyer:                " << 0.001 * distance(a, r, strategy::distance::andoyer<lola>() ) << " km" << std::endl;
	std::cout << "vincenty:               " << 0.001 * distance(a, r, strategy::distance::vincenty<lola>() ) << " km" << std::endl;
	std::cout << "vincenty rad:           " << 0.001 * distance(a_rad, r_rad, strategy::distance::vincenty<lolar>() ) << " km" << std::endl;
	std::cout << "RD, pythagoras:         " << 0.001 * distance(a_rd, r_rd) << " km" << std::endl;

	std::cout << std::endl;
	std::cout << "Distance Amsterdam-Den Haag: " << std::endl;
	std::cout << "haversine:              " << 0.001 * distance(a, h) << " km" << std::endl;

	// ------------------------------------------------------------------------------------------
	// Distances to segments
	// ------------------------------------------------------------------------------------------
	std::cout << std::endl << "The Hague - line Amsterdam,Rotterdam" << std::endl;

	const_segment<XY> ar_xy(a_rd, r_rd);

	// Using distance algorithm - not implemented for segments
	// std::cout << distance(h_rd, ar_xy) << std::endl;

	// Using default strategy for point_xy
	strategy_traits<XY>::point_segment_distance calcxy;

	distance_result dr = calcxy(h_rd, ar_xy);
	std::cout << "in RD: " << 0.001 * dr << std::endl;

	// Using default strategy for point_ll
	strategy_traits<lola>::point_segment_distance calcll;
	dr = calcll(h, const_segment<lola>(a, r));
	std::cout << "in LL: " << 0.001 * dr << std::endl;

	std::cout << std::endl << "Rotterdam - line Amsterdam,the Hague" << std::endl;
	dr = calcxy(r_rd, const_segment<XY>(a_rd, h_rd));
	std::cout << "in RD: " << 0.001 * dr << std::endl;	
	dr = calcll(r, const_segment<lola>(a, h));
	std::cout << "in LL: " << 0.001 * dr << std::endl;
	std::cout << std::endl;


	// ------------------------------------------------------------------------------------------
	// Compilation
	// ------------------------------------------------------------------------------------------
	// Next line does not compile because Vincenty cannot work on xy-points
	//std::cout << "vincenty on xy:         " << 0.001 * distance(a_rd, r_rd, formulae::distance::vincenty<>() ) << " km" << std::endl;

	// Next line does not compile because you cannot (yet) assign degree to radian directly
	//lolar a_rad2 = a;

	// Next line does not compile because you cannot assign latlong to xy
	// XY axy = a;


	// ------------------------------------------------------------------------------------------
	// Length
	// ------------------------------------------------------------------------------------------
	// Length calculations use distances internally. The lines below take automatically the default
	// formulae for distance. However, you can also specify a formula explicitly.
	typedef linestring<lola> lll;
	typedef linestring<XY> lxy;

	lll line1;
	line1.push_back(a);
	line1.push_back(r);
	std::cout << "length: " << length(line1) << std::endl;
	std::cout << "length using Vincenty: " << length(line1, strategy::distance::vincenty<lola>()) << std::endl;

	lxy line2;
	line2.push_back(a_rd);
	line2.push_back(r_rd);
	std::cout << "length: " << length(line2) << std::endl;

	return 0;
}
