// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_IS_GRAPH_HPP
#define BOOST_MGL_IS_GRAPH_HPP

#include <boost/mgl/directed_graph.hpp>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/detail/bool_trait_def.hpp>

namespace boost
{

namespace mgl
{

namespace detail
{

template<typename T>
struct is_graph_impl :
	is_base_of<directed_graph<T>, T>::type
{
};

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_graph, T, ::boost::mgl::detail::is_graph_impl<T>::value)

} // namespace mgl

} // namespace boost

#include <boost/type_traits/detail/bool_trait_undef.hpp>

#endif // BOOST_MGL_IS_GRAPH_HPP
