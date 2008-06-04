// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <deque>
#include <iterator>
#include <algorithm> // for reverse, unique

#include <geometry/geometry.hpp>
#include <geometry/envelope.hpp>
#include <geometry/segment.hpp>

#include <geometry/distance.hpp>
#include <geometry/length.hpp>
#include <geometry/centroid.hpp>
#include <geometry/intersection.hpp>
#include <geometry/simplify.hpp>

#include <geometry/foreach.hpp>

#include <geometry/streamwkt.hpp>



class derived_linestring : public geometry::linestring<geometry::point_xy<float>, std::deque, std::allocator>
{
	public :
		typedef geometry::box<point_type> box_type;
		inline void rebox() { geometry::envelope(*this, m_box); }
		inline box_type box() const { return m_box; }
	private :
		box_type m_box;

};

struct own_linestring : public std::vector<geometry::point_xy<double> >
{
	typedef geometry::point_xy<double> point_type; // required for, at least, the simplify_linestring algorithm
};



template<typename P>
std::ostream & operator<<(std::ostream &s, const geometry::box<P> &b)
{
	s << b.min() << " - " << b.max();
	return s;
}


template<typename P>
inline void translate_function(P& p)
{
		p.x(p.x() + 100.0);
}


template<typename P>
struct scale_functor
{
	inline void operator()(P& p)
	{
		p.x(p.x() * 1000.0);
		p.y(p.y() * 1000.0);
	}
};



int main(void)
{
	// Define a linestring, which is a vector of points, and add some points
	geometry::linestring<geometry::point_xy<double> > ls;
	ls.push_back(geometry::point_xy<double>(1.1, 1.1));

	geometry::point_xy<double> lp(2.5, 2.1);
	ls.push_back(lp);

	// Lines can be streamed as Well Known Text (OGC WKT)
	std::cout << ls << std::endl;

	// The bounding box of lines can be calculated and streamd useing the method above
	geometry::box<geometry::point_xy<double> > b;
	geometry::envelope(ls, b);
	std::cout << b << std::endl;

	// The length of the line can be calulated
	std::cout << "length: " << geometry::length(ls) << std::endl;

	// All things from vector can be called, because a linestring is a vector
	std::cout << "number of points: " << ls.size() << std::endl;

	// The distance from a point to a linestring can be calculated
	geometry::point_xy<double> p(1.9, 1.2);
	std::cout << "distance of " << p << " to line: " << geometry::distance(p, ls) << std::endl;

	// A linestring is a vector. However, some algorithms consider "segments",
	// which are the line pieces between two points of a linestring.
	// Here we go to the "primary versions"
	//geometry::distance_result d = geometry::impl::distance::point_to_segment(
	//	p, geometry::const_segment<geometry::point_xy<double> >(ls.front(), ls.back()));
	//std::cout << sqrt(geometry::square_distance_point_to_segment(p, geometry::const_segment<geometry::point_xy<double> >(ls.front(), ls.back()))) << std::endl;

	// Add some points more, let's do it using a classic array. See documentation for picture
	typedef geometry::point_xy<double> P;
	const double c[][2] = { {3.1, 3.1}, {4.9, 1.1}, {3.1, 1.9} };
	const int n = sizeof(c) / sizeof(c[0]);
	for (int i = 0; i < n; i++)
	{
		ls.push_back(P(c[i][0], c[i][1]));
	}
	std::cout << "original: " << ls << std::endl;

	// All algorithms from std can be used: a linestring is a vector
	std::reverse(ls.begin(), ls.end());
	std::cout << "reversed: " << ls << std::endl;
	std::reverse(ls.begin(), ls.end());

	// The other way, using a vector instead of a linestring, is also possible
	std::vector<P> pv(ls.begin(), ls.end());
	// However, you can NOT call length then, because length is defined for linestring<P>
	// All methods are templatized in two ways, so it is still possible to get the length
	// but with another name. This is like "find" and "find_if"
	std::cout << "length: " << geometry::length(pv.begin(), pv.end()) << std::endl;

	// If there are double points in the line, you can use unique to remove them
	// So we add the last point, print, make a unique copy and print
	ls.push_back(ls.back());
	std::cout << "extra point: " << ls << std::endl;

	{
		geometry::linestring<P> ls_copy;
		std::unique_copy(ls.begin(), ls.end(), std::back_inserter(ls_copy));
		ls = ls_copy;
		std::cout << "uniquecopy: " << ls << std::endl;
	}

	// Lines can be simplified using e.g. Douglas Peucker
	geometry::linestring<P> ls_simplified;
	geometry::simplify(ls, ls_simplified, 0.5);
	std::cout << "simplified: " << ls_simplified << std::endl;

	// Declare and fill a derived linestring
	derived_linestring dl;
	for (geometry::linestring<P>::const_iterator it = ls.begin(); it != ls.end(); it++)
	{
		dl.push_back(geometry::point_xy<float>(it->x(), it->y()));
	}
	// In this case it has its own box
	dl.rebox();
	std::cout << dl.box() << std::endl;

	derived_linestring dls;
	// Simplify also works for derived classes
	geometry::simplify(dl, dls, 0.5);

	own_linestring ol;
	for (geometry::linestring<P>::const_iterator it = ls.begin(); it != ls.end(); it++)
	{
		ol.push_back(geometry::point_xy<double>(it->x(), it->y()));
	}
	own_linestring ols;

	// A call to simplify does NOT work for own classes...
	//   geometry::simplify(ol, ols, 0.5); // does NOT work
	// ... so call simplify_linestring here
	//geometry::impl::simplify::do_linestring(ol, ols, 0.5);


	// for_each:
	// 1) Lines can be visited with std::for_each
	// 2) geometry::for_each_point is also defined for all geometries
	// 3) geometry::for_each_segment is defined for all geometries to all segments
	// 4) geometry::loop is defined for geometries to visit segments 
	//    with state apart, and to be able to break out (not shown here)
	{
		geometry::linestring<geometry::point_xy<double> > lscopy = ls;
		std::for_each(ls.begin(), ls.end(), translate_function<P>);
		geometry::for_each_point(ls, scale_functor<P>());
		std::cout << ls << std::endl;
		ls = lscopy;
		std::cout << ls << std::endl;
	}

	// Finally, lines can be clipped using a clipping box. Clipped lines are added to the output iterator
	geometry::box<P> cb(P(1.5, 1.5), P(4.5, 2.5));
	std::ostream_iterator<geometry::linestring<geometry::point_xy<double> > > out(std::cout, "\n");
	geometry::intersection(cb, ls, out);


	return 0;
}
