/**
 * \file boost/mirror/algorithm/for_each.hpp
 * Run-time application of a functor to all base_classes/attributes/etc.
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
	template <class meta_object_sequence>
	struct for_each_impl 
	: iterative_algorithm<meta_object_sequence, for_each_meta_object> 
	{ };

} // namespace detail

template <
	class meta_object_sequence,
	class functor
>
static inline functor for_each(functor fn)
{
	return detail::for_each_impl<meta_object_sequence> ::perform(fn, detail::no_op());
}

template <
	class meta_object_sequence,
	class transform_op,
	class functor
>
static inline functor for_each(transform_op transf, functor fn)
{
	return detail::for_each_impl<meta_object_sequence> ::perform(fn, transf);
}

} // namespace mirror
} // namespace boost

#endif //include guard

