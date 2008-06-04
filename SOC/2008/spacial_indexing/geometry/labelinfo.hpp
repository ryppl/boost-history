// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_LABELINFO_HPP
#define _GEOMETRY_LABELINFO_HPP


// Algorithms to generate appropriate labelpoint(s) for all geometries



#include <geometry/geometry.hpp>
#include <geometry/centroid.hpp>

namespace geometry
{

	// For a polygon the broadest line is probably the best, so two points (left/right)
	// For a line either the centerpoint, of the longest line, or the most horizontal line,
	//   or more than two points to smooth the text around some points. So one/two or more points.
	// For a point just return the point.

	// The algorithms output to an output iterator
	// They have a label_option to influence behaviour. Not yet implemented.
	// Is there a better approach? Class?


	enum label_option {label_default,
			// line
			label_longest, label_horizontal,
			// polygon
			label_centroid, label_broadest
	};


	template<typename P, typename MP>
	inline void label_info_point(const P& point, label_option option, MP& mp)
	{
		mp.resize(1);
		mp.front().x(point.x());
		mp.front().y(point.y());
	}


	template<typename Y, typename MP>
	inline void label_info_polygon(const Y& poly, label_option option, MP& mp)
	{
		mp.resize(0);
		mp.push_back(centroid_polygon<typename MP::point_type>(poly));
	}

	template<typename B, typename MP>
	inline void label_info_box(const B& box, label_option option, MP& mp)
	{
		mp.resize(0);
		mp.push_back(centroid_box(box));
	}




	//-------------------------------------------------------------------------------------------------------
	// General "label_info" versions
	//-------------------------------------------------------------------------------------------------------
	template<typename P, typename MP>
	inline void label_info(const P& p, label_option option, MP& mp)
	{
		label_info_point(p, option, mp);
	}



	template<typename P, typename MP>
	inline void label_info(const box<P>& b, label_option option, MP& mp)
	{
		label_info_box(b, option, mp);
	}


	template<typename P,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR,
			typename MP>
	inline void label_info(const polygon<P, VP, VR, AP, AR>& poly, label_option option, MP& mp)
	{
		label_info_polygon(poly, option, mp);
	}

} // namespace geometry


#endif // _GEOMETRY_LABELINFO_HPP
