/**
 * \file boost/mirror/algorithm/for_each.hpp
 * Run-time application of a Functor to all base_classes/attributes/etc.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_FOR_EACH_HPP
#define BOOST_MIRROR_ALGORITHM_FOR_EACH_HPP

#include <boost/mirror/algorithm/detail/for_each.hpp>
#include <boost/mirror/algorithm/detail/iterative.hpp>
#include <boost/mirror/algorithm/detail/no_op.hpp>

namespace boost {
namespace mirror {
namespace detail {

	/** Declaration of the default for_each_impl
	 *  helper template.
	 */
	template <class MetaObjectSequence>
	struct for_each_impl 
	: iterative_algorithm<MetaObjectSequence, for_each_meta_object> 
	{ };

} // namespace detail

template <
	class MetaObjectSequence,
	class Functor
>
static inline Functor for_each(Functor fn)
{
	return detail::for_each_impl<MetaObjectSequence> ::perform(fn, detail::no_op());
}

template <
	class MetaObjectSequence,
	class TransformOp,
	class Functor
>
static inline Functor for_each(TransformOp transf, Functor fn)
{
	return detail::for_each_impl<MetaObjectSequence> ::perform(fn, transf);
}

} // namespace mirror
} // namespace boost

#endif //include guard

