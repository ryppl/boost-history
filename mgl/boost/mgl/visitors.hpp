// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_VISITORS_HPP
#define BOOST_MGL_VISITORS_HPP

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/contains.hpp>

#include <boost/mgl/colors.hpp>

#include <boost/type_traits/is_same.hpp>

namespace boost
{

namespace mgl
{

BOOST_MPL_HAS_XXX_TRAIT_DEF(discover_vertex)
BOOST_MPL_HAS_XXX_TRAIT_DEF(examine_edge)

struct null_visitor
{
	struct on_init
	{
		typedef ::boost::mpl::void_ type;
	};
};

struct vertex_trace_visitor
{
	typedef ::boost::mpl::true_ discover_vertex;

	struct on_init
	{
		typedef ::boost::mpl::vector0<> type;
	};

	template<typename Vertex, typename T, typename TraversalStack, typename ColorMap>
	struct on_discover_vertex
	{
		typedef typename ::boost::mpl::push_back<T, Vertex>::type type;
	};
};

struct vertex_counter_visitor
{
	typedef ::boost::mpl::true_ discover_vertex;

	struct on_init
	{
		typedef ::boost::mpl::int_<0> type;
	};

	template<typename Vertex, typename T, typename TraversalStack, typename ColorMap>
	struct on_discover_vertex
	{
		typedef typename ::boost::mpl::plus<T, ::boost::mpl::int_<1> >::type type;
	};
};

struct edge_trace_visitor
{
	typedef ::boost::mpl::true_ examine_edge;

	struct on_init
	{
		typedef ::boost::mpl::vector0<> type;
	};

	template<typename Edge, typename T, typename TraversalStack, typename ColorMap>
	struct on_examine_edge
	{
		typedef typename ::boost::mpl::push_back<T, Edge>::type type;
	};
};

struct edge_counter_visitor
{
	typedef ::boost::mpl::true_ examine_edge;

	struct on_init
	{
		typedef ::boost::mpl::int_<0> type;
	};

	template<typename Edge, typename T, typename TraversalStack, typename ColorMap>
	struct on_examine_edge
	{
		typedef typename ::boost::mpl::plus<T, ::boost::mpl::int_<1> >::type type;
	};
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_VISITORS_HPP
