#include <geometry/area.hpp>
#include <geometry/aswkt.hpp>
#include <geometry/centroid.hpp>
#include <geometry/correct.hpp>
#include <geometry/distance.hpp>
#include <geometry/envelope.hpp>
#include <geometry/fromwkt.hpp>
#include <geometry/intersection_segment.hpp>
#include <geometry/intersection_linestring.hpp>
#include <geometry/latlong.hpp>
#include <geometry/length.hpp>
#include <geometry/strategies/strategies_point_ll.hpp>
#include <geometry/streamwkt.hpp>
#include <geometry/simplify.hpp>
#include <geometry/within.hpp>

#include <iostream>

// All functions below are referred to in the source of the Geometry Library.
// Don't rename them.

void example_area_polygon()
{
	geometry::polygon<geometry::point_xy<double> > poly;
	geometry::from_wkt("POLYGON((0 0,0 7,4 2,2 0,0 0))", poly);
	std::cout << "Polygon area is "
		<< geometry::area(poly)
		<< " square units" << std::endl;

	geometry::polygon<geometry::point_ll<geometry::degree, float> > poly_ll;
	geometry::from_wkt("POLYGON((4 51,4 52,5 52,5 51,4 51))", poly_ll);
	std::cout << "Polygon area is "
		<< geometry::area(poly_ll)/(1000*1000)
		<< " square kilometers " << std::endl;
}

void example_as_wkt_point()
{
	typedef geometry::point_xy<double> P;
	P p(5.12, 6.34);
	// Points can be streamed like this:
	std::cout << geometry::as_wkt<P>(p) << std::endl;

	// or, including streamwkt, like this:
	std::cout << p << std::endl;
}

void example_centroid_polygon()
{
	geometry::polygon<geometry::point_xy<double> > poly;
	geometry::from_wkt("POLYGON((0 0,0 7,4 2,2 0,0 0))", poly);
	geometry::point_xy<float> center;
	geometry::centroid(poly, center);
	std::cout << "Centroid: " << center.x() << "," << center.y() << std::endl;
}


void example_distance_point_point()
{
	geometry::point_xy<double> p1(1, 1);
	geometry::point_xy<double> p2(2, 3);
	std::cout << "Distance p1-p2 is " 
		<< geometry::distance(p1, p2) 
		<< " units" << std::endl;

	// Read 2 Dutch cities from WKT texts (in decimal degrees)
	geometry::point_ll<geometry::degree, double> a, r;
	geometry::from_wkt("POINT(4.89222 52.3731)", a);
	geometry::from_wkt("POINT(4.47917 51.9308)", r);

	std::cout << "Distance Amsterdam-Rotterdam is "
		<< geometry::distance(a, r) / 1000.0 
		<< " kilometers " << std::endl;
}

void example_distance_point_point_strategy()
{
	typedef geometry::point_ll<geometry::degree, double> LL;
	LL a, r;
	geometry::from_wkt("POINT(4.89222 52.3731)", a);
	geometry::from_wkt("POINT(4.47917 51.9308)", r);

	std::cout << "Distance Amsterdam-Rotterdam is "
		<< geometry::distance(a, r,
				geometry::strategy::distance::vincenty<LL>() )
				 / 1000.0
		<< " kilometers " << std::endl;
}

void example_from_wkt_point()
{
	geometry::point_xy<int> point;
	geometry::from_wkt("Point(1 2)", point);
	std::cout << point.x() << "," << point.y() << std::endl;
}

void example_from_wkt_output_iterator()
{
	std::vector<geometry::point_xy<int> > v;
	geometry::from_wkt<geometry::point_xy<int> >("linestring(1 1,2 2,3 3,4 4)", std::back_inserter(v));
	std::cout << "vector has " << v.size() << " coordinates" << std::endl;
}

void example_from_wkt_linestring()
{
	geometry::linestring<geometry::point_xy<double> > line;
	geometry::from_wkt("linestring(1 1,2 2,3 3,4 4)", line);
	std::cout << "linestring has " << line.size() << " coordinates" << std::endl;
}

void example_from_wkt_polygon()
{
	geometry::polygon<geometry::point_xy<double> > poly;
	geometry::from_wkt("POLYGON((0 0,0 1,1 1,1 0,0 0))", poly);
	std::cout << "Polygon has " << poly.outer().size() << " coordinates in outer ring" << std::endl;
}

void example_point_ll_convert()
{
	geometry::point_ll<geometry::degree, double> deg(geometry::latitude<>(33.0), geometry::longitude<>(-118.0));
	geometry::point_ll<geometry::radian, double> rad = deg.convert<geometry::radian>();

	std::cout << "point in radians: " << rad << std::endl;
}

void example_intersection_linestring1()
{
	typedef geometry::point_xy<double> P;
	geometry::linestring<P> line;
	geometry::from_wkt("linestring(1.1 1.1, 2.5 2.1, 3.1 3.1, 4.9 1.1, 3.1 1.9)", line);
	geometry::box<P> cb(P(1.5, 1.5), P(4.5, 2.5));
	std::cout << "Clipped linestring(s) " << std::endl;
	geometry::intersection(cb, line,
			std::ostream_iterator<geometry::linestring<P> >(std::cout, "\n"));
}

void example_intersection_segment1()
{
	typedef geometry::point_xy<double> P;
	P a(0, 2);
	P b(4, 2);
	P c(3, 0);
	P d(3, 4);
	geometry::segment<P> s1(a, b);
	geometry::segment<P> s2(c, d);

	std::cout << "Intersection point(s): ";
	geometry::intersection_result r = geometry::intersection_segment<P>(s1, s2, 
		std::ostream_iterator<P>(std::cout, "\n"));
	std::cout << "Intersection result: " << r.is_type << std::endl;
}

void example_simplify_linestring1()
{
	geometry::linestring<geometry::point_xy<double> > line, simplified;
	geometry::from_wkt("linestring(1.1 1.1, 2.5 2.1, 3.1 3.1, 4.9 1.1, 3.1 1.9)", line);
	geometry::simplify(line, simplified, 0.5);
	std::cout
		<< "  original line: " << line << std::endl
		<< "simplified line: " << simplified << std::endl;
}

void example_simplify_linestring2()
{
	typedef geometry::point_xy<double> P;
	typedef std::ostream_iterator<P> OUT;
	typedef geometry::strategy::distance::xy_point_segment<P> DS;
	typedef geometry::linestring<P> L;
	L line;
	geometry::from_wkt("linestring(1.1 1.1, 2.5 2.1, 3.1 3.1, 4.9 1.1, 3.1 1.9)", line);
	geometry::strategy::simplify::douglas_peucker<L, OUT, DS> simplification;
	geometry::simplify(line.begin(), line.end(), OUT(std::cout, "\n"), 0.5, simplification, DS());
}

void example_simplify_linestring3()
{
	typedef geometry::point_xy<double> P;
	typedef std::ostream_iterator<P> OUT;
	typedef geometry::strategy::distance::xy_point_segment<P> DS;
	std::vector<P> line;
	geometry::from_wkt<P>("linestring(1.1 1.1, 2.5 2.1, 3.1 3.1, 4.9 1.1, 3.1 1.9)", std::back_inserter(line));
	geometry::strategy::simplify::douglas_peucker<std::vector<P>, OUT, DS> simplification;
	geometry::simplify(line.begin(), line.end(), OUT(std::cout, "\n"), 0.5, simplification, DS());
}


void example_within()
{
	geometry::polygon<geometry::point_xy<double> > poly;
	geometry::from_wkt("POLYGON((0 0,0 7,4 2,2 0,0 0))", poly);
	geometry::point_xy<float> point(3, 3);
	std::cout << "Point is "
		<< (geometry::within(point, poly) ? "IN" : "NOT in")
		<< " polygon"
		<< std::endl;
}

/*
void example_within_strategy()
{
	// TO BE UPDATED/FINISHED
	typedef geometry::point_xy<double> P;
	typedef geometry::polygon<P> POLY;
	P p;
	std::cout << within(p, poly, strategy::within::cross_count<P>) << std::endl;
}
*/

void example_length_linestring()
{
	using namespace geometry;
	linestring<point_xy<double> > line;
	from_wkt("linestring(0 0,1 1,4 8,3 2)", line);
	std::cout << "linestring length is "
		<< length(line)
		<< " units" << std::endl;

	// Linestring in latlong, filled with 
	// explicit degree-minute-second values
	typedef point_ll<degree, float> LL;
	linestring<LL> line_ll;
	line_ll.push_back(LL(
		latitude<float>(dms<north, float>(52, 22, 23)), 
		longitude<float>(dms<east, float>(4, 53, 32))));
	line_ll.push_back(LL(
		latitude<float>(dms<north, float>(51, 55, 51)), 
		longitude<float>(dms<east, float>(4, 28, 45))));
	line_ll.push_back(LL(
		latitude<float>(dms<north, float>(52, 4, 48)), 
		longitude<float>(dms<east, float>(4, 18, 0))));
	std::cout << "linestring length is "
		<< length(line_ll) / 1000
		<< " kilometers " << std::endl;
}

void example_length_linestring_iterators1()
{
	geometry::linestring<geometry::point_xy<double> > line;
	geometry::from_wkt("linestring(0 0,1 1,4 8,3 2)", line);
	std::cout << "linestring length is "
		<< geometry::length(line.begin(), line.end())
		<< " units" << std::endl;
}

void example_length_linestring_iterators2()
{
	std::vector<geometry::point_xy<double> > line;
	geometry::from_wkt<geometry::point_xy<double> >("linestring(0 0,1 1,4 8,3 2)", std::back_inserter(line));
	std::cout << "linestring length is "
		<< geometry::length(line.begin(), line.end())
		<< " units" << std::endl;
}

void example_length_linestring_iterators3()
{
	using namespace geometry;
	typedef point_ll<degree, float> LL;
	std::deque<LL> line;
	geometry::from_wkt<LL>("linestring(0 51,1 51,2 52)", std::back_inserter(line));
	std::cout << "linestring length is "
		<< 0.001 * geometry::length(line.begin(), line.end(), geometry::strategy::distance::vincenty<LL>())
		<< " kilometers" << std::endl;
}


void example_length_linestring_strategy()
{
	using namespace geometry;
	typedef point_ll<degree, float> LL;
	linestring<LL> line_ll;
	line_ll.push_back(LL(latitude<float>(dms<north, float>(52, 22, 23)), longitude<float>(dms<east, float>(4, 53, 32))));
	line_ll.push_back(LL(latitude<float>(dms<north, float>(51, 55, 51)), longitude<float>(dms<east, float>(4, 28, 45))));
	line_ll.push_back(LL(latitude<float>(dms<north, float>(52, 4, 48)), longitude<float>(dms<east, float>(4, 18, 0))));
	std::cout << "linestring length is "
		<< length(line_ll, strategy::distance::vincenty<LL, LL>() )/(1000)
		<< " kilometers " << std::endl;
}


void example_envelope_linestring()
{
	geometry::linestring<geometry::point_xy<double> > line;
	geometry::from_wkt("linestring(0 0,1 1,4 8,3 2)", line);
	geometry::box<geometry::point_xy<double> > box;
	geometry::envelope(line, box);

	std::cout << "box is " << box.min() << "-" << box.max() << std::endl;
}

void example_envelope_polygon()
{
	using namespace geometry;
	typedef point_ll<degree, double>  LL;

	// Wrangel island, 180 meridian crossing island above Siberia.
	polygon<LL> wrangel;
	wrangel.outer().push_back(LL(latitude<>(dms<north>(70, 47, 7)), longitude<>(dms<west>(178, 47, 9))));
	wrangel.outer().push_back(LL(latitude<>(dms<north>(71, 14, 0)), longitude<>(dms<east>(177, 28, 33))));
	wrangel.outer().push_back(LL(latitude<>(dms<north>(71, 34, 24)), longitude<>(dms<east>(179, 44, 37))));
	// Close it
	wrangel.outer().push_back(wrangel.outer().front());

	geometry::box<LL> box;
	geometry::envelope(wrangel, box);

	dms<cd_lat> minlat(box.min().lat());
	dms<cd_lon> minlon(box.min().lon());

	dms<cd_lat> maxlat(box.max().lat());
	dms<cd_lon> maxlon(box.max().lon());

	std::cout << wrangel << std::endl;
	std::cout << "min: " << minlat.get_dms() << " , " << minlon.get_dms() << std::endl;
	std::cout << "max: " << maxlat.get_dms() << " , " << maxlon.get_dms() << std::endl;
}


void example_dms()
{
	// Construction with degree/minute/seconds
	geometry::dms<geometry::east> d1(4, 53, 32.5);

	// Explicit conversion to double. 
	std::cout << d1.as_value() << std::endl;

	// Conversion to string, with optional strings
	std::cout << d1.get_dms(" deg ", " min ", " sec") << std::endl;

	// Combination with latitude/longitude and cardinal directions
	{
		using namespace geometry;
		point_ll<degree> canberra(
			latitude<>(dms<south>(35, 18, 27)), 
			longitude<>(dms<east>(149, 7, 27.9)));
		std::cout << canberra << std::endl;
	}
}

void example_point_ll_construct()
{
	using namespace geometry;
	typedef point_ll<degree> ll;

	// Constructions in both orders possible
	ll juneau(
		latitude<>(dms<north>(58, 21, 5)), 
		longitude<>(dms<west>(134, 30, 42)));
	ll wladiwostok(
		longitude<>(dms<east>(131, 54)),
		latitude<>(dms<north>(43, 8))
		);
}

namespace example_loop1
{
	// Class functor
	template <typename S>
	struct perimeter
	{
		struct summation
		{
			double sum;
			summation() : sum(0) {}
		};

		bool operator()(const S& segment, summation& s) const
		{
			std::cout << "from " << segment.first << " to " << segment.second << std::endl;
			s.sum += geometry::distance(segment.first, segment.second);
			return true;
		}
	};

	void example()
	{
		typedef geometry::point_xy<double> P;
		typedef geometry::const_segment<P> S;
		geometry::polygon<P> poly;
		geometry::from_wkt("POLYGON((0 0,0 7,4 2,2 0,0 0))", poly);
		perimeter<S>::summation peri;
		geometry::loop(poly.outer(), perimeter<S>(), peri);
		std::cout << "Perimeter: " << peri.sum << std::endl;
	}
} //:\\


namespace example_loop2
{
	struct summation
	{
		double sum;
		summation() : sum(0) {}
	};

	// Function functor
	template <typename S>
	bool perimeter(const S& segment, summation& s) 
	{
		std::cout << "from " << segment.first << " to " << segment.second << std::endl;
		s.sum += geometry::distance(segment.first, segment.second);
		return true;
	}

	void example()
	{
		typedef geometry::point_ll<geometry::degree, double> P;
		typedef geometry::const_segment<P> S;
		geometry::polygon<P> poly;
		geometry::from_wkt("POLYGON((-178.786 70.7853,177.476 71.2333,179.744 71.5733,-178.786 70.7853))", poly);
		summation peri;
		geometry::loop(poly.outer(), perimeter<S>, peri);
		std::cout << "Perimeter: " << peri.sum/1000.0 << " km" << std::endl;
	}
} //:\\


int main(void)
{
	example_area_polygon();

	example_centroid_polygon();

	example_distance_point_point();
	example_distance_point_point_strategy();

	example_from_wkt_point();
	example_from_wkt_output_iterator();
	example_from_wkt_linestring();
	example_from_wkt_polygon();

	example_as_wkt_point();

	example_intersection_linestring1();
	example_intersection_segment1();

	example_simplify_linestring1();
	example_simplify_linestring2();
	example_simplify_linestring3();

	example_length_linestring();
	example_length_linestring_iterators1();
	example_length_linestring_iterators2();
	example_length_linestring_iterators3();
	example_length_linestring_strategy();

	example_envelope_linestring();
	example_envelope_polygon();

	example_within();

	example_point_ll_convert();
	example_point_ll_construct();
	example_dms();

	example_loop1::example();
	example_loop2::example();

	return 0;
}
