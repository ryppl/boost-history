// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_BEGIN_END_HPP
#define BOOST_MGL_BEGIN_END_HPP

#include <boost/mpl/has_xxx.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mgl/aux_/begin_end_impl.hpp>
#include <boost/mgl/iterator_policies.hpp>
#include <boost/mgl/visitors.hpp>

namespace boost
{

namespace mgl
{

BOOST_MPL_HAS_XXX_TRAIT_DEF(dfs_begin_supported)
BOOST_MPL_HAS_XXX_TRAIT_DEF(bfs_begin_supported)

/// \brief Get a DFS iterator to the begin of a graph
/// \param Graph Graph where the iteration should be performed
/// \param EndAtStrategy The strategy used to abort the iteration
/// \param TracePolicy The policy used to trace the iteration
/// \param Visitor The visitor class used during the iteration
template<class Graph, class EndAtStrategy = ::boost::mgl::EndAtEndOfGraph, class TracePolicy = ::boost::mgl::NoTrace, class Visitor = ::boost::mgl::null_visitor, class Enable = void>
struct dfs_begin
{
	typedef ::boost::mpl::void_ type;
};

template<class Graph, class EndAtStrategy, class TracePolicy, class Visitor>
struct dfs_begin<Graph, EndAtStrategy, TracePolicy, Visitor, typename ::boost::enable_if<has_dfs_begin_supported<Graph> >::type>
{
	typedef typename Graph::template dfs_begin<EndAtStrategy, TracePolicy, Visitor>::type type;
};

/// \brief Get a DFS iterator to the end of a graph
/// \param Graph Graph where the iteration should be performed
template<class Graph, class Enable = void>
struct dfs_end
{
	typedef ::boost::mpl::void_ type;
};

template<class Graph>
struct dfs_end<Graph, typename ::boost::enable_if<has_dfs_begin_supported<Graph> >::type>
{
	typedef typename Graph::end::type type;
};

/// \brief Get a BFS iterator to the begin of a graph
/// \param Graph Graph where the iteration should be performed
/// \param EndAtStrategy The strategy used to abort the iteration
/// \param TracePolicy The policy used to trace the iteration
/// \param Visitor The visitor class used during the iteration
template<class Graph, class EndAtStrategy = ::boost::mgl::EndAtEndOfGraph, class TracePolicy = ::boost::mgl::NoTrace, class Visitor = ::boost::mgl::null_visitor, class Enable = void>
struct bfs_begin
{
	typedef ::boost::mpl::void_ type;
};

template<class Graph, class EndAtStrategy, class TracePolicy, class Visitor>
struct bfs_begin<Graph, EndAtStrategy, TracePolicy, Visitor, typename ::boost::enable_if<has_bfs_begin_supported<Graph> >::type>
{
	typedef typename Graph::template bfs_begin<EndAtStrategy, TracePolicy, Visitor>::type type;
};

/// \brief Get a BFS iterator to the end of a graph
/// \param Graph Graph where the iteration should be performed
template<class Graph, class Enable = void>
struct bfs_end
{
	typedef ::boost::mpl::void_ type;
};

template<class Graph>
struct bfs_end<Graph, typename ::boost::enable_if<has_bfs_begin_supported<Graph> >::type>
{
	typedef typename Graph::end::type type;
};

//! @todo Add enable_if's like above!!!
template<class VertexIterator>
struct edge_begin
{
	typedef typename aux::edge_begin_impl::template apply<typename VertexIterator::graph, VertexIterator>::type type;
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_BEGIN_END_HPP
