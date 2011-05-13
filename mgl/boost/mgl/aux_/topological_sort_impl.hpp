// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_TOPOLOGICAL_SORT_IMPL_HPP
#define BOOST_MGL_TOPOLOGICAL_SORT_IMPL_HPP

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/contains.hpp>

#include <boost/mgl/edge.hpp>

namespace boost
{

namespace mgl
{

namespace aux
{

BOOST_MPL_HAS_XXX_TRAIT_DEF(examine_edge)

struct predecessor_edge_builder
{
	typedef ::boost::mpl::true_ examine_edge;

	struct on_init
	{
		typedef ::boost::mpl::map0<> type;
	};

	template<typename Edge, typename T, typename TraversalStack, typename ColorMap>
	struct on_examine_edge
	{
		typedef typename ::boost::mpl::if_<
			typename ::boost::mpl::has_key<T, typename ::boost::mpl::first<Edge>::type>::type,
			T,
			typename ::boost::mpl::insert<
				T,
				::boost::mpl::pair<
					typename ::boost::mpl::first<Edge>::type,
					::boost::mpl::vector0<>
				>
			>::type
		>::type T_;

		typedef typename ::boost::mpl::if_<
			typename ::boost::mpl::has_key<T_, typename ::boost::mpl::second<Edge>::type>::type,
			typename ::boost::mpl::at<T, typename ::boost::mpl::second<Edge>::type>::type,
			::boost::mpl::vector0<>
		>::type bucket;

		typedef typename ::boost::mpl::push_back<
			bucket,
			typename ::boost::mpl::first<Edge>::type
		>::type new_bucket;

		typedef typename ::boost::mpl::erase_key<
			T_,
			typename ::boost::mpl::second<Edge>::type
		>::type T__;

		typedef typename ::boost::mpl::insert<
			T__,
			::boost::mpl::pair<
				typename ::boost::mpl::second<Edge>::type,
				new_bucket
			>
		>::type type;
	};
};

template<typename Vector, typename ExcludedElements>
struct build_predecessors : ::boost::mpl::fold<
		Vector,
		::boost::mpl::vector0<>,
		::boost::mpl::if_<
			::boost::mpl::contains<
				ExcludedElements,
				::boost::mpl::placeholders::_2
			>,
			::boost::mpl::placeholders::_1,
			::boost::mpl::push_back<
				::boost::mpl::placeholders::_1,
				::boost::mpl::placeholders::_2
			>
		>
	>
{};

template<class Graph, class Predecessors, class Result> 
struct topological_sort_impl
{
	// find empty basket and add them to the result vector
	typedef typename ::boost::mpl::fold<
		Predecessors,
		Result,
		::boost::mpl::if_<
			::boost::mpl::empty<
				::boost::mpl::second< ::boost::mpl::placeholders::_2>
			>,
			::boost::mpl::push_back<
				::boost::mpl::placeholders::_1,
				::boost::mpl::first< ::boost::mpl::placeholders::_2>
			>,
			::boost::mpl::placeholders::_1
		>
	>::type result;

	// delete the empty baskets
	typedef typename ::boost::mpl::fold<
		result,
		Predecessors,
		::boost::mpl::erase_key<
			::boost::mpl::placeholders::_1,
			::boost::mpl::placeholders::_2
		>
	>::type predecessors;

	// build a new predecessor map
	typedef typename ::boost::mpl::fold<
		predecessors,
		::boost::mpl::map0<>,
		::boost::mpl::insert<
			::boost::mpl::placeholders::_1,
			::boost::mpl::pair<
				::boost::mpl::first< ::boost::mpl::placeholders::_2>,
				build_predecessors<
					::boost::mpl::second< ::boost::mpl::placeholders::_2>,
					result
				>
			>
		>
	>::type new_predecessors;

	typedef typename ::boost::mpl::eval_if<
		typename ::boost::mpl::empty<new_predecessors>,
		result,
		topological_sort_impl<Graph, new_predecessors, result>
	>::type type;
};

} // namespace aux

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_TOPOLOGICAL_SORT_IMPL_HPP
