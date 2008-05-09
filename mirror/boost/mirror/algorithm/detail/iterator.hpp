/**
 * \file boost/mirror/algorithm/detail/iterator.hpp
 * Includes all meta-object sequence iterators
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_ITERATOR_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_ITERATOR_HPP

#include <boost/mirror/algorithm/detail/attribute_iterator.hpp>
#include <boost/mirror/algorithm/detail/base_class_iterator.hpp>

namespace boost {
namespace mirror {
namespace detail {

	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class Position,
		class UnaryPredicate
	>
	struct meta_object_iterator
	: meta_object_iterator_base<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		Position,
		UnaryPredicate,
		iterator_pointee_selector<MetaObjectSequence>
	>{ };

	/** Gets the initial iterator for the given meta_attributes
	 *  and unary predicate.
	 */
	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class UnaryPredicate
	>
	struct get_meta_object_iterator
	{
			typedef typename mpl::apply<
			typename meta_object_iterator<
				ReflectedType, 
				VariantTag,
				MetaObjectSequence,
				mpl::int_<0>,
				UnaryPredicate
			>::get_initial_iterator,
			mpl::int_<0>
		>::type type;
	};



} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

