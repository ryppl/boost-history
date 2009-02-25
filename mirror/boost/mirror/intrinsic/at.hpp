/**
 * \file boost/mirror/intrinsic/at.hpp
 * Gets the I-th meta-attribute/meta-inheritence/etc.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INTRINSIC_AT_HPP
#define BOOST_MIRROR_INTRINSIC_AT_HPP

#include <boost/mirror/intrinsic/detail/attribute_at.hpp>
#include <boost/mirror/intrinsic/detail/base_class_at.hpp>
#include <boost/mirror/detail/global_list.hpp>
#include <boost/mpl/at.hpp>

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

	/** Specialization of at_impl<MetaObjectSequence>
	 *  for meta_class_all_attributes<>
	 */
	template <class Class, class VariantTag, class Position>
	struct at_impl<meta_class_all_attributes<Class, VariantTag>, Position >
	: meta_attribute_at<
		Class, VariantTag,
		meta_class_all_attributes<Class, VariantTag>,
		Position
	>{ };

	/** Specialization of at_impl<MetaObjectSequence>
	 *  for meta_base_classes<>
	 */
	template <class Class, class VariantTag, class Position>
	struct at_impl<meta_base_classes<Class, VariantTag>, Position >
	: meta_inheritance_at<
		meta_base_classes<Class, VariantTag>,
		Position
	>{ };

	/** Specialization of at_impl<MetaObjectSequence>
	 *  for global_list<>
	 */
	template <class Selector, class Counter, class Filter, class Position>
	struct at_impl<counter::filtered_global_list<Selector, Counter, Filter>, Position >
	: mpl::at<
		typename counter::filtered_global_list<Selector, Counter, Filter>::type,
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

