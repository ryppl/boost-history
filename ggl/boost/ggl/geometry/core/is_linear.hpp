// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef _GEOMETRY_IS_LINEAR_HPP
#define _GEOMETRY_IS_LINEAR_HPP


#include <boost/type_traits.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <geometry/core/tag.hpp>
#include <geometry/core/tags.hpp>


namespace geometry
{


	#ifndef DOXYGEN_NO_DISPATCH
	namespace core_dispatch
	{
		template <typename TAG, typename G>
		struct is_linear : boost::false_type {};


		template <typename G> struct is_linear<linestring_tag, G> : boost::true_type {};
		template <typename G> struct is_linear<ring_tag, G>       : boost::true_type {};


	} // namespace core_dispatch
	#endif




	/*!
		\brief Meta-function defining "true" for linear types (linestring,ring), "false" for non-linear typse
		\details Can be used for tag dispatching mechanism
		\ingroup core
	*/
	template <typename G>
	struct is_linear
		: core_dispatch::is_linear
			< typename tag<G>::type , typename boost::remove_const<G>::type >
	{ };


}


#endif
