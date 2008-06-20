/**
 * \file boost/mirror/meta_path/for_each.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_PATH_FOR_EACH_HPP
#define BOOST_MIRROR_META_PATH_FOR_EACH_HPP

#include <boost/mirror/meta_path/node_context.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/ref.hpp>

namespace boost {
namespace mirror {

namespace meta_path {

/** Execute a functor on nodes in the given nodeset
 */
template <
	class NodeSet, 
	class Functor
>
void for_each(Functor f)
{
	mpl::for_each<
		typename NodeSet::paths_and_nodes,
	>(f);
}


/** Execute a functor on the transformed nodes in the given nodeset
 */
template <
	class NodeSet, 
	class TransformOp, 
	class Functor
>
void for_each(Functor f)
{
	mpl::for_each<
		typename NodeSet::paths_and_nodes,
		TransformOp
	>(f);
}

/** Execute a functor on nodes in the given nodeset
 */
template <
	class NodeSet, 
	class Functor
>
void for_each(boost::reference_wrapper<Functor> f_ref)
{
	mpl::for_each<
		typename NodeSet::paths_and_nodes,
	>(f_ref);
}


/** Execute a functor on the transformed nodes in the given nodeset
 */
template <
	class NodeSet, 
	class TransformOp, 
	class Functor
>
void for_each(boost::reference_wrapper<Functor> f_ref)
{
	mpl::for_each<
		typename NodeSet::paths_and_nodes,
		TransformOp
	>(f_ref);
}


} // namespace meta_path
} // namespace mirror
} // namespace boost

#endif //include guard

