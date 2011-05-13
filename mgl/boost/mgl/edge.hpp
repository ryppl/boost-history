// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_EDGE_HPP
#define BOOST_MGL_EDGE_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>

namespace boost
{

namespace mgl
{

template<class VertexFrom, class VertexTo, int Weight, class Properties = ::boost::mpl::vector0<> >
struct edge
{
	typedef VertexFrom vertex_from;
	typedef VertexTo vertex_to;
	typedef ::boost::mpl::int_<Weight> weight;
	typedef Properties properties;
};

template<class Edge>
struct edge_begin_vertex
{
	typedef typename Edge::vertex_from type;
};

template<class Edge>
struct edge_end_vertex
{
	typedef typename Edge::vertex_to type;
};

template<class Edge>
struct edge_weight
{
	typedef typename Edge::weight type;
};

template<class Edge>
struct edge_properties
{
	typedef typename Edge::properties type;
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_EDGE_HPP
