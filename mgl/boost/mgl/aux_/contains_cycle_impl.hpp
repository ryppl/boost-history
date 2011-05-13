// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_AUX_CONTAINS_CYCLE_IMPL_HPP
#define BOOST_MGL_AUX_CONTAINS_CYCLE_IMPL_HPP

#include <boost/mgl/next_prior.hpp>

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/pair.hpp>

#include <boost/type_traits/is_same.hpp>

namespace boost
{

namespace mgl
{

namespace aux
{

BOOST_MPL_HAS_XXX_TRAIT_DEF(examine_edge)

struct cycle_detector_visitor
{
	typedef ::boost::mpl::true_ examine_edge;
	
	struct on_init
	{
		typedef ::boost::mpl::false_ type;
	};

	template<typename Edge, typename T, typename TraversalStack, typename ColorMap>
	struct on_examine_edge
	{
		typedef typename ::boost::mpl::eval_if<
			::boost::is_same<T, ::boost::mpl::false_>,
			::boost::mpl::contains<TraversalStack, typename ::boost::mpl::second<Edge>::type>,
			::boost::mpl::true_
		>::type type;
	};
};

} // namespace aux

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_AUX_CONTAINS_CYCLE_IMPL_HPP
