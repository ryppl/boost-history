// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_CONTAINS_CYCLE_HPP
#define BOOST_MGL_CONTAINS_CYCLE_HPP

#include <boost/mpl/vector.hpp>

#include <boost/mgl/begin_end.hpp>
#include <boost/mgl/depth_first_search.hpp>
#include <boost/mgl/iterator_policies.hpp>

#include <boost/mgl/aux_/contains_cycle_impl.hpp>

namespace boost
{

namespace mgl
{

template<class Graph>
struct contains_cycle
{
	typedef typename ::boost::mgl::dfs_begin<
		Graph,
		::boost::mgl::EndWhenNoVerticesFound,
		::boost::mgl::RecordTrace< ::boost::mpl::vector0<> >,
		aux::cycle_detector_visitor
	>::type iter;

	typedef typename aux::template depth_first_search_impl<iter>::type::visitor_result type;
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_CONTAINS_CYCLE_HPP
