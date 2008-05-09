/**
 * \file boost/mirror/algorithm/begin.hpp
 * Returns the iterator pointing to the first meta-object
 * in a meta-object sequence.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_BEGIN_HPP
#define BOOST_MIRROR_ALGORITHM_BEGIN_HPP

#include <boost/mirror/algorithm/detail/begin.hpp>

namespace boost {
namespace mirror {
namespace detail {

	/** Declaration of the default begin_impl
	 *  helper template.
	 */
	template <class MetaObjectSequence>
	struct begin_impl { };

	/** Specialization of begin_impl<MetaObjectSequence>
	 *  for meta_class_attributes<>
	 */
	template <class Class, class VariantTag>
	struct begin_impl<meta_class_attributes<Class, VariantTag> >
	: meta_object_sequence_begin<
		Class, VariantTag,
		meta_class_attributes<Class, VariantTag>
	>{ };

	/** Specialization of for_each_impl<MetaObjectSequence>
	 *  for meta_class_all_attributes<>
	 */
	template <class Class, class VariantTag>
	struct begin_impl<meta_class_all_attributes<Class, VariantTag> >
	: meta_object_sequence_begin<
		Class, VariantTag,
		meta_class_all_attributes<Class, VariantTag>
	>{ };

	/** Specialization of for_each_impl<MetaObjectSequence>
	 *  for meta_base_classes<>
	 */
	template <class Class, class VariantTag>
	struct begin_impl<meta_base_classes<Class, VariantTag> >
	: meta_object_sequence_begin<
		Class, VariantTag,
		meta_base_classes<Class, VariantTag>
	>{ };

} // namespace detail

template <class MetaObjectSequence>
struct begin : detail::begin_impl<MetaObjectSequence>
{ };

} // namespace mirror
} // namespace boost

#endif //include guard

