// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_VISITOR_HELPERS_HPP
#define BOOST_MGL_VISITOR_HELPERS_HPP

namespace boost
{

namespace mgl
{

template<class Visitor, class Vertex, class VisitorResult, class TraversalStack, class ColorMap>
struct do_on_discover_vertex
{
	typedef typename Visitor::template on_discover_vertex<
		Vertex,
		VisitorResult,
		TraversalStack,
		ColorMap
	>::type type;
};

template<class Visitor, class Vertex1, class Vertex2, class VisitorResult, class TraversalStack, class ColorMap>
struct do_on_examine_edge
{
	typedef typename Visitor::template on_examine_edge<
		::boost::mpl::pair<Vertex1, Vertex2>,
		VisitorResult,
		TraversalStack,
		ColorMap
	>::type type;
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_VISITOR_HELPERS_HPP
