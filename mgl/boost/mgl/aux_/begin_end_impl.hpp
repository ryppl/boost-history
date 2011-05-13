// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_AUX_BEGIN_END_IMPL_HPP
#define BOOST_MGL_AUX_BEGIN_END_IMPL_HPP

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/has_xxx.hpp>

#include <boost/mgl/is_graph.hpp>
#include <boost/mgl/iterator.hpp>
#include <boost/mgl/begin_end.hpp>

namespace boost
{

namespace mgl
{

namespace aux
{

//template<typename Graph, typename EndAtStrategy, typename TracePolicy, typename Visitor>
//struct dfs_begin_type
//{
//	typedef typename Graph::template dfs_begin<EndAtStrategy, TracePolicy, Visitor>::type type;
//};
//
//template<typename Graph>
//struct dfs_end_type
//{
//	typedef typename Graph::end::type type;
//};
//
//BOOST_MPL_HAS_XXX_TRAIT_DEF(dfs_begin_supported)
//
//template<typename Tag>
//struct dfs_begin_impl
//{
//	template<typename Graph, typename EndAtStrategy, typename TracePolicy, typename Visitor> struct apply
//	{
//		typedef typename ::boost::mpl::eval_if<
//			typename has_dfs_begin_supported<Graph>::type,
//			//is_graph<Graph>,
//			aux::dfs_begin_type<Graph, EndAtStrategy, TracePolicy, Visitor>,
//			::boost::mpl::void_
//		>::type type;
//	};
//};
//
//template<typename Tag>
//struct dfs_end_impl
//{
//	template<typename Graph> struct apply
//	{
//		typedef typename ::boost::mpl::eval_if<
//			typename has_dfs_begin_supported<Graph>::type,
//			aux::dfs_end_type<Graph>,
//			::boost::mpl::void_
//		>::type type;
//	};
//};

//template<typename Graph, typename EndAtStrategy, typename TracePolicy, typename Visitor>
//struct bfs_begin_type
//{ 
//	typedef typename Graph::template bfs_begin<EndAtStrategy, TracePolicy, Visitor>::type type;
//};
//
//template<typename Graph> 
//struct bfs_end_type
//{ 
//	typedef typename Graph::end::type type;
//};
//
//BOOST_MPL_HAS_XXX_TRAIT_DEF(bfs_begin_supported)
//
//template<typename Tag>
//struct bfs_begin_impl
//{
//	template<typename Graph, typename EndAtStrategy, typename TracePolicy, typename Visitor> struct apply
//	{
//		typedef typename ::boost::mpl::eval_if<
//			//typename has_bfs_begin_supported<Graph>::type,
//			is_graph<Graph>,
//			aux::bfs_begin_type<Graph, EndAtStrategy, TracePolicy, Visitor>,
//			::boost::mpl::void_
//		>::type type;
//	};
//};
//
//template<typename Tag>
//struct bfs_end_impl
//{
//	template<typename Graph> struct apply
//	{
//		typedef typename ::boost::mpl::eval_if<
//			typename has_bfs_begin_supported<Graph>::type,
//			aux::bfs_end_type<Graph>,
//			::boost::mpl::void_
//		>::type type;
//	};
//};

template<typename Graph, typename VertexIterator>
struct edge_begin_type
{
	typedef typename Graph::template edge_begin<VertexIterator>::type type;
};

struct edge_begin_impl
{
	template<typename Graph, typename VertexIterator> struct apply
	{
		typedef typename aux::edge_begin_type<Graph, VertexIterator>::type type;
	};
};

} // namespace aux

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_AUX_BEGIN_END_IMPL_HPP
