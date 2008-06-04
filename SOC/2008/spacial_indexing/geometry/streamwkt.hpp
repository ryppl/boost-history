// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_STREAMWKT_HPP
#define _GEOMETRY_STREAMWKT_HPP

#include <geometry/aswkt.hpp>


namespace geometry
{

	/*!
		\brief Streams a point as Well-Known Text
		\ingroup wkt
	*/
	template <typename CH, typename TR, typename T, size_t D>
	inline std::basic_ostream<CH,TR>& operator<<(std::basic_ostream<CH,TR> &os, const point<T, D> &p)
	{
		os << as_wkt<point<T, D> >(p);
		return os;
	}

	/*!
		\brief Streams a linestring as Well-Known Text
		\ingroup wkt
	*/
	template<typename CH, typename TR, typename P, template<typename,typename> class V, template<typename> class A>
	inline std::basic_ostream<CH,TR>& operator<<(std::basic_ostream<CH,TR> &os, const linestring<P, V, A> &ls)
	{
		os << as_wkt<linestring<P, V, A> >(ls);
		return os;
	}

	/*!
		\brief Streams a polygon as Well-Known Text
		\ingroup wkt
	*/
	template<typename CH, typename TR, typename P,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR>
	inline std::basic_ostream<CH,TR>& operator<<(std::basic_ostream<CH,TR> &os, const polygon<P, VP, VR, AP, AR>& poly)
	{
		os << as_wkt<polygon<P, VP, VR, AP, AR> >(poly);
		return os;
	}

}


#endif // _GEOMETRY_ASTEXT_HPP
