// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_ITERATOR_HPP
#define BOOST_MGL_ITERATOR_HPP

#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/mgl/next_prior.hpp>
#include <boost/mgl/iterator_tags.hpp>
#include <boost/mgl/iterator_policies.hpp>
#include <boost/mgl/visitors.hpp>
#include <boost/mgl/colors.hpp>

namespace boost
{

namespace mgl
{

template<class Graph, class Vertex, class ColorMap, class TraversalStack, class EndAtStrategy = ::boost::mgl::EndAtEndOfGraph, class TracePolicy = ::boost::mgl::NoTrace, class Trace = ::boost::mpl::void_, class Visitor = ::boost::mgl::null_visitor, class VisitorResult = ::boost::mpl::void_>
struct dfs_iterator
{
	typedef Graph graph;
	typedef Vertex vertex;

	typedef ColorMap color_map;

	typedef TraversalStack traversal_stack;

	typedef ::boost::mgl::dfs_iterator_tag category;

	typedef EndAtStrategy end_at_strategy;

	typedef TracePolicy trace_policy;
	typedef Trace vertex_trace;

	typedef Visitor visitor_type;
	typedef VisitorResult visitor_result;
};

template<class Graph, class Vertex, class ColorMap, class TraversalStack, class EndAtStrategy = ::boost::mgl::EndAtEndOfGraph, class TracePolicy = ::boost::mgl::NoTrace, class Trace = ::boost::mpl::void_, class Visitor = ::boost::mgl::null_visitor, class VisitorResult = ::boost::mpl::void_>
struct bfs_iterator
{
	typedef Graph graph;
	typedef Vertex vertex;

	typedef ColorMap color_map;

	typedef TraversalStack traversal_stack;

	typedef ::boost::mgl::bfs_iterator_tag category;

	typedef EndAtStrategy end_at_strategy;

	typedef TracePolicy trace_policy;
	typedef Trace vertex_trace;

	typedef Visitor visitor_type;
	typedef VisitorResult visitor_result;
};

template<class Graph, class Vertex, class TargetVertex, class EdgeIndex = ::boost::mpl::int_<0> >
struct edge_iterator
{
	typedef Graph graph;

	typedef Vertex source_vertex;
	typedef TargetVertex target_vertex;

	typedef EdgeIndex edge_index;
};

/// \brief Dereference the iterator
/// \param Iterator Iterator to be dereferences
template<class Iterator>
struct deref
{
	typedef typename Iterator::type type;
};

template<class Graph, class Vertex, class VisitedSet, class TraversalStack, class EndAtStrategy, class TracePolicy, class Trace, class Visitor, class VisitorResult>
struct deref<dfs_iterator<Graph, Vertex, VisitedSet, TraversalStack, EndAtStrategy, TracePolicy, Trace, Visitor, VisitorResult> >
{
	typedef Vertex type;
};

template<class Graph, class Vertex, class VisitedSet, class TraversalStack, class EndAtStrategy, class TracePolicy, class Trace, class Visitor, class VisitorResult>
struct deref<bfs_iterator<Graph, Vertex, VisitedSet, TraversalStack, EndAtStrategy, TracePolicy, Trace, Visitor, VisitorResult> >
{
	typedef Vertex type;
};

template<class Graph, class Vertex, class TargetVertex, class EdgeIndex>
struct deref<edge_iterator<Graph, Vertex, TargetVertex, EdgeIndex> >
{
	typedef TargetVertex type;
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_ITERATOR_HPP
