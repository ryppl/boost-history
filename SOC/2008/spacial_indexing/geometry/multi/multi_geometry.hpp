// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_MULTI_GEOMETRY_HPP
#define _GEOMETRY_MULTI_GEOMETRY_HPP

#include <vector>

#include <geometry/geometry.hpp>



namespace geometry
{

	// Class: multi_point
	// Purpose: groups points belonging to each other, e.g. a constellation
	template<typename P,
			template<typename,typename> class V = std::vector,
			template<typename> class A = std::allocator>
	class multi_point : public V<P, A<P> >, public geometry_traits<P>
	{
	};

	// Class: multi_line
	// Purpose: groups lines belonging to each other, e.g. a broken highway
	template<typename L,
			template<typename,typename> class V = std::vector,
			template<typename> class A = std::allocator>
	class multi_linestring : public V<L, A<L> >, public geometry_traits<typename L::point_type>
	{
		public:
			typedef L linestring_type;
	};

	// Class: multi_polygon
	// Purpose: groups polygons belonging to each other, e.g. Hawaii
	template<typename Y,
			template<typename,typename> class V = std::vector,
			template<typename> class A = std::allocator>
	class multi_polygon : public V<Y, A<Y> >, public geometry_traits<typename Y::point_type>
	{
		public:
			typedef Y polygon_type;
	};


} // namespace geometry


#endif // _GEOMETRY_MULTI_GEOMETRY_HPP
