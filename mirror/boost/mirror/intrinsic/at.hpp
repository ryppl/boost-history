/**
 * \file boost/mirror/algorithm/at.hpp
 * Gets the I-th meta-attribute/meta-inheritence/etc.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_AT_HPP
#define BOOST_MIRROR_ALGORITHM_AT_HPP

#include <boost/mirror/algorithm/detail/attribute_at.hpp>
#include <boost/mirror/algorithm/detail/base_class_at.hpp>

namespace boost {
namespace mirror {
namespace detail {

	/** Declaration of the default at_impl
	 *  helper template.
	 */
	template <class MetaObjectSequence, class Position>
	struct at_impl { };

	/** Specialization of at_impl<MetaObjectSequence>
	 *  for meta_class_attributes<>
	 */
	template <class Class, class VariantTag, class Position>
	struct at_impl<meta_class_attributes<Class, VariantTag>, Position >
	: meta_attribute_at<
		Class, VariantTag,
		meta_class_attributes<Class, VariantTag>,
		Position
	>{ };

	/** Specialization of for_each_impl<MetaObjectSequence>
	 *  for meta_class_all_attributes<>
	 */
	template <class Class, class VariantTag, class Position>
	struct at_impl<meta_class_all_attributes<Class, VariantTag>, Position >
	: meta_attribute_at<
		Class, VariantTag,
		meta_class_all_attributes<Class, VariantTag>,
		Position
	>{ };

	/** Specialization of for_each_impl<MetaObjectSequence>
	 *  for meta_base_classes<>
	 */
	template <class Class, class VariantTag, class Position>
	struct at_impl<meta_base_classes<Class, VariantTag>, Position >
	: meta_inheritance_at<
		meta_base_classes<Class, VariantTag>,
		Position
	>{ };

} // namespace detail

template <
	class MetaObjectSequence,
	class Position
>
struct at : detail::at_impl<MetaObjectSequence, Position>
{ };

} // namespace mirror
} // namespace boost

#endif //include guard

