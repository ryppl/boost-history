/**
 * \file boost/mirror/algorithm/end.hpp
 * Returns the iterator pointing to the first meta-object
 * in a meta-object sequence.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_END_HPP
#define BOOST_MIRROR_ALGORITHM_END_HPP

#include <boost/mirror/algorithm/detail/end.hpp>

namespace boost {
namespace mirror {
namespace detail {

	/** Declaration of the default end_impl
	 *  helper template.
	 */
	template <class MetaObjectSequence>
	struct end_impl { };

	/** Specialization of end_impl<MetaObjectSequence>
	 *  for meta_class_attributes<>
	 */
	template <class Class, class VariantTag>
	struct end_impl<meta_class_attributes<Class, VariantTag> >
	: meta_object_sequence_end<
		Class, VariantTag,
		meta_class_attributes<Class, VariantTag>
	>{ };

	/** Specialization of for_each_impl<MetaObjectSequence>
	 *  for meta_class_all_attributes<>
	 */
	template <class Class, class VariantTag>
	struct end_impl<meta_class_all_attributes<Class, VariantTag> >
	: meta_object_sequence_end<
		Class, VariantTag,
		meta_class_all_attributes<Class, VariantTag>
	>{ };

	/** Specialization of for_each_impl<MetaObjectSequence>
	 *  for meta_base_classes<>
	 */
	template <class Class, class VariantTag>
	struct end_impl<meta_base_classes<Class, VariantTag> >
	: meta_object_sequence_end<
		Class, VariantTag,
		meta_base_classes<Class, VariantTag>
	>{ };

} // namespace detail

template <class MetaObjectSequence>
struct end : detail::end_impl<MetaObjectSequence>
{ };

} // namespace mirror
} // namespace boost

#endif //include guard

