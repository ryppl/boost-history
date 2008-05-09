/**
 * \file boost/mirror/algorithm/find_if.hpp
 *
 * Returns the iterator pointing to the first meta-object
 * in a meta-object sequence satisfying the given predicate.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_FIND_IF_HPP
#define BOOST_MIRROR_ALGORITHM_FIND_IF_HPP

#include <boost/mirror/algorithm/detail/find_if.hpp>

namespace boost {
namespace mirror {
namespace detail {

	/** Declaration of the default find_if_impl
	 *  helper template.
	 */
	template <class MetaObjectSequence, class Predicate>
	struct find_if_impl { };

	/** Specialization of find_if_impl<MetaObjectSequence, class Predicate>
	 *  for meta_class_attributes<>
	 */
	template <class Class, class VariantTag, class Predicate>
	struct find_if_impl<meta_class_attributes<Class, VariantTag>, Predicate>
	: meta_object_find_if<
		Class, VariantTag,
		meta_class_attributes<Class, VariantTag>,
		Predicate
	>{ };

	/** Specialization of for_each_impl<MetaObjectSequence>
	 *  for meta_class_all_attributes<>
	 */
	template <class Class, class VariantTag, class Predicate>
	struct find_if_impl<meta_class_all_attributes<Class, VariantTag>, Predicate>
	: meta_object_find_if<
		Class, VariantTag,
		meta_class_all_attributes<Class, VariantTag>,
		Predicate
	>{ };

	/** Specialization of for_each_impl<MetaObjectSequence>
	 *  for meta_base_classes<>
	 */
	template <class Class, class VariantTag, class Predicate>
	struct find_if_impl<meta_base_classes<Class, VariantTag>, Predicate>
	: meta_object_find_if<
		Class, VariantTag,
		meta_base_classes<Class, VariantTag>,
		Predicate
	>{ };

} // namespace detail

template <class MetaObjectSequence, class Predicate>
struct find_if : detail::find_if_impl<MetaObjectSequence, Predicate>
{ };

} // namespace mirror
} // namespace boost

#endif //include guard

