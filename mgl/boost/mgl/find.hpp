// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_FIND_HPP
#define BOOST_MGL_FIND_HPP

#include <boost/mpl/has_xxx.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mgl/iterator_policies.hpp>
#include <boost/mgl/visitors.hpp>

namespace boost
{

namespace mgl
{

BOOST_MPL_HAS_XXX_TRAIT_DEF(dfs_find_supported)
BOOST_MPL_HAS_XXX_TRAIT_DEF(bfs_find_supported)

/// \brief Find a specified vertex inside a graph and return a DFS iterator.
/// \param Graph Graph where the iteration should be performed
/// \param Vertex Vertex that should be found inside the graph
/// \param EndAtStrategy The strategy used to abort the iteration
/// \param TracePolicy The policy used to trace the iteration
/// \param Visitor The visitor class used during the iteration
//template<class Graph, class Vertex, class EndAtStrategy = ::boost::mgl::EndAtEndOfGraph, class TracePolicy = ::boost::mgl::NoTrace, class Visitor = ::boost::mgl::null_visitor>
template<class Graph, class Vertex, class EndAtStrategy = ::boost::mgl::EndAtEndOfGraph, class TracePolicy = ::boost::mgl::NoTrace, class Visitor = ::boost::mgl::null_visitor, class Enable = void>
struct dfs_find
{
	typedef ::boost::mpl::void_ type;
};

template<class Graph, class Vertex, class EndAtStrategy, class TracePolicy, class Visitor>
struct dfs_find<Graph, Vertex, EndAtStrategy, TracePolicy, Visitor, typename ::boost::enable_if<has_dfs_find_supported<Graph> >::type>
{
	typedef typename Graph::template dfs_find<Vertex, EndAtStrategy, TracePolicy, Visitor>::type type;
};

/// \brief Find a specified vertex inside a graph and return a BFS iterator.
/// \param Graph Graph where the iteration should be performed
/// \param Vertex Vertex that should be found inside the graph
/// \param EndAtStrategy The strategy used to abort the iteration
/// \param TracePolicy The policy used to trace the iteration
/// \param Visitor The visitor class used during the iteration
template<class Graph, class Vertex, class EndAtStrategy = ::boost::mgl::EndAtEndOfGraph, class TracePolicy = ::boost::mgl::NoTrace, class Visitor = ::boost::mgl::null_visitor, class Enable = void>
struct bfs_find
{
	typedef ::boost::mpl::void_ type;
};

template<class Graph, class Vertex, class EndAtStrategy, class TracePolicy, class Visitor>
struct bfs_find<Graph, Vertex, EndAtStrategy, TracePolicy, Visitor, typename ::boost::enable_if<has_bfs_find_supported<Graph> >::type>
{
	typedef typename Graph::template bfs_find<Vertex, EndAtStrategy, TracePolicy, Visitor>::type type;
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_FIND_HPP
