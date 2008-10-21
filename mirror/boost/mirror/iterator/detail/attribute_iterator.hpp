/**
 * \file boost/mirror/iterator/detail/attribute_iterator.hpp
 * Iterators for iterating through meta-object sequences
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ITERATOR_DETAIL_ATTRIBUTE_ITERATOR_HPP
#define BOOST_MIRROR_ITERATOR_DETAIL_ATTRIBUTE_ITERATOR_HPP

#include <boost/mirror/iterator/detail/common_iterator.hpp>
#include <boost/mirror/meta_attributes.hpp>

namespace boost {
namespace mirror {
namespace detail {

	struct attribute_iterator_pointee_selector
	{
		template <
			class ReflectedType, 
			class VariantTag,
			class MetaAttributes,
			class AttribPos,
			class UnaryPredicate
		>
		struct apply
		{
			typedef meta_class_attribute<
				ReflectedType, 
				VariantTag,
				MetaAttributes,
				AttribPos
			> type;
		};
	};

	template <class ReflectedType, class VariantTag>
	struct iterator_pointee_selector<
		meta_class_attributes<ReflectedType, VariantTag>
	> : attribute_iterator_pointee_selector
	{ };

	template <class ReflectedType, class VariantTag>
	struct iterator_pointee_selector<
		meta_class_all_attributes<ReflectedType, VariantTag>
	> : attribute_iterator_pointee_selector
	{ };


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

