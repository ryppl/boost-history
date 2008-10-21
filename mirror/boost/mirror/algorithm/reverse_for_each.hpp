/**
 * \file boost/mirror/algorithm/reverse_for_each.hpp
 * Run-time application of a Functor to all base_classes/attributes/etc.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_REVERSE_FOR_EACH_HPP
#define BOOST_MIRROR_ALGORITHM_REVERSE_FOR_EACH_HPP

#include <boost/mirror/algorithm/detail/reverse_for_each.hpp>
#include <boost/mirror/algorithm/detail/no_op.hpp>
#include <boost/mirror/intrinsic/begin.hpp>
#include <boost/mirror/intrinsic/end.hpp>

namespace boost {
namespace mirror {
namespace detail {

	/** Declaration of the default reverse_for_each_impl
	 *  helper template.
	 */
	template <class IteratorBegin, class IteratorEnd>
	struct reverse_for_each_impl 
	: iterative_algorithm<
		IteratorBegin,
		IteratorEnd,
		reverse_for_each_meta_object
	>{ };

} // namespace detail

template <
	class MetaObjectSequence,
	class Functor
>
static reference_wrapper<Functor> reverse_for_each(
	reference_wrapper<Functor> fn
)
{
	return detail::reverse_for_each_impl<
		typename mirror::begin<MetaObjectSequence>::type,
		typename mirror::end<MetaObjectSequence>::type
	> ::perform(fn, cref(detail::no_op()));
}

template <
	class MetaObjectSequence,
	class Functor
>
static Functor reverse_for_each(Functor fn)
{
	return detail::reverse_for_each_impl<
		typename mirror::begin<MetaObjectSequence>::type,
		typename mirror::end<MetaObjectSequence>::type
	> ::perform(ref(fn), cref(detail::no_op()));
}

template <
	class MetaObjectSequence,
	class TransformOp,
	class Functor
>
static reference_wrapper<Functor> reverse_for_each(
	reference_wrapper<TransformOp> transf, 
	reference_wrapper<Functor> fn
)
{
	return detail::reverse_for_each_impl<
		typename mirror::begin<MetaObjectSequence>::type,
		typename mirror::end<MetaObjectSequence>::type
	> ::perform(fn, transf);
}

template <
	class MetaObjectSequence,
	class TransformOp,
	class Functor
>
static Functor reverse_for_each(
	TransformOp transf, 
	Functor fn
)
{
	return detail::reverse_for_each_impl<
		typename mirror::begin<MetaObjectSequence>::type,
		typename mirror::end<MetaObjectSequence>::type
	> ::perform(ref(fn), ref(transf));
}

template <
	class IteratorBegin,
	class IteratorEnd,
	class Functor
>
static reference_wrapper<Functor> reverse_for_each(
	reference_wrapper<Functor> fn
)
{
	return detail::reverse_for_each_impl<
		IteratorBegin,
		IteratorEnd
	> ::perform(fn, cref(detail::no_op()));
}

template <
	class IteratorBegin,
	class IteratorEnd,
	class TransformOp,
	class Functor
>
static Functor reverse_for_each(
	reference_wrapper<TransformOp> transf, 
	reference_wrapper<Functor> fn
)
{
	return detail::reverse_for_each_impl<
		IteratorBegin,
		IteratorEnd
	> ::perform(fn, transf);
}



} // namespace mirror
} // namespace boost

#endif //include guard

