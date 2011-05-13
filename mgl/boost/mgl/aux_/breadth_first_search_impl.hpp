// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_BREADTH_FIRST_SEARCH_IMPL_HPP
#define BOOST_MGL_BREADTH_FIRST_SEARCH_IMPL_HPP

#include <boost/mgl/begin_end.hpp>
#include <boost/mgl/next_prior.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost
{

namespace mgl
{

namespace aux
{

template<class Iterator, class AtEnd = void>
struct breadth_first_search_impl
{
	typedef typename breadth_first_search_impl<
		typename ::boost::mgl::next<Iterator>::type
	>::type type;
};

template<class Iterator>
struct breadth_first_search_impl<Iterator, typename ::boost::enable_if< ::boost::is_same<Iterator, ::boost::mpl::void_> >::type>
{
	typedef bfs_iterator<
		void,
		void,
		::boost::mpl::set0<>,
		::boost::mpl::vector0<>
	> type;

	typedef typename ::boost::mpl::void_ visitor_type;
	typedef typename ::boost::mpl::void_ visitor_result;
};

template<class Iterator>
struct breadth_first_search_impl<Iterator, typename ::boost::enable_if< ::boost::is_same<typename ::boost::mgl::dfs_end<typename Iterator::graph>::type, typename Iterator::vertex> >::type>
{
	typedef bfs_iterator<
		typename Iterator::graph,
		typename Iterator::graph::end,
		typename Iterator::color_map,
		typename Iterator::traversal_stack,
		typename Iterator::end_at_strategy,
		typename Iterator::trace_policy,
		typename Iterator::vertex_trace,
		typename Iterator::visitor_type,
		typename Iterator::visitor_result
	> type;
};

} // namespace aux

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_BREATH_FIRST_SEARCH_IMPL_HPP
