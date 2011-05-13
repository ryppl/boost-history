// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_TOPOLOGICAL_SORT_HPP
#define BOOST_MGL_TOPOLOGICAL_SORT_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/void.hpp>

#include <boost/mgl/depth_first_search.hpp>
#include <boost/mgl/visitors.hpp>

#include <boost/mgl/aux_/topological_sort_impl.hpp>

namespace boost
{

namespace mgl
{

template<class Graph>
struct topological_sort
{
	typedef typename depth_first_search<
		Graph,
		aux::predecessor_edge_builder
	>::type::visitor_result predecessor_map;

	BOOST_MPL_ASSERT_NOT(( ::boost::mpl::empty<predecessor_map> ));

	typedef typename aux::topological_sort_impl<
		Graph,
		predecessor_map,
		::boost::mpl::vector0<>
	>::type type;

	BOOST_MPL_ASSERT_NOT(( ::boost::mpl::empty<type> ));
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_TOPOLOGICAL_SORT_HPP
