// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_AUX_NEXT_PRIOR_IMPL_HPP
#define BOOST_MGL_AUX_NEXT_PRIOR_IMPL_HPP

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/mgl/iterator_tags.hpp>

namespace boost
{

namespace mgl
{

namespace aux
{

template<class Tag>
struct next_impl
{ 
	template<class Iterator, class Enable = void> struct apply
	{
		// Return void if not supported iterator type is given.
		typedef ::boost::mpl::void_ type;
	};

	template<class Iterator>
	struct apply<Iterator, typename ::boost::enable_if<typename ::boost::is_same<typename Iterator::category, dfs_iterator_tag> >::type >
	{
		typedef typename Iterator::graph::template dfs_next<Iterator>::type type;
	};

	template<class Iterator>
	struct apply<Iterator, typename ::boost::enable_if<typename ::boost::is_same<typename Iterator::category, bfs_iterator_tag> >::type >
	{
		typedef typename Iterator::graph::template bfs_next<Iterator>::type type;
	};
};

template<class Tag>
struct prior_impl
{
	//! @todo Implement me!
};

template<class Tag>
struct edge_next_impl
{
	template<typename Iterator> struct apply
	{
		typedef typename Iterator::graph::template edge_next<Iterator>::type type;
	};
};

} // namespace aux

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_AUX_NEXT_PRIOR_IMPL_HPP
