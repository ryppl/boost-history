// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_UTIL_RESERVE_HPP
#define _GEOMETRY_UTIL_RESERVE_HPP

#include <list>
#include <deque>

namespace geometry
{
	// We would like to have a "reserve" on list. Fixed as a generic function, with overloads/partial specializations

	template <typename T, template<typename> class A, template<typename,typename> class V>
	void container_reserve(V<T, A<T> >& container, int n)
	{
		container.reserve(n);
	}

	// Partially specialize for list such that calls to container_reserve are OK now
	template <typename T, template<typename> class A>
	void container_reserve(std::list<T, A<T> >, int n)
	{
	}
	template <typename T, template<typename> class A>
	void container_reserve(std::deque<T, A<T> >, int n)
	{
	}
} // namespace geometry

#endif // _GEOMETRY_UTIL_RESERVE_HPP
