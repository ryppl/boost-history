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
	template <class meta_object_sequence, class position>
	struct at_impl { };

	/** Specialization of at_impl<meta_object_sequence>
	 *  for meta_class_attributes<>
	 */
	template <class reflected_class, class variant_tag, class position>
	struct at_impl<meta_class_attributes<reflected_class, variant_tag>, position >
	: meta_attribute_at<
		reflected_class, variant_tag,
		meta_class_attributes<reflected_class, variant_tag>,
		position
	>{ };

	/** Specialization of for_each_impl<meta_object_sequence>
	 *  for meta_class_all_attributes<>
	 */
	template <class reflected_class, class variant_tag, class position>
	struct at_impl<meta_class_all_attributes<reflected_class, variant_tag>, position >
	: meta_attribute_at<
		reflected_class, variant_tag,
		meta_class_all_attributes<reflected_class, variant_tag>,
		position
	>{ };

	/** Specialization of for_each_impl<meta_object_sequence>
	 *  for meta_base_classes<>
	 */
	template <class reflected_class, class variant_tag, class position>
	struct at_impl<meta_base_classes<reflected_class, variant_tag>, position >
	: meta_inheritance_at<
		meta_base_classes<reflected_class, variant_tag>,
		position
	>{ };

} // namespace detail

template <
	class meta_object_sequence,
	class position
>
struct at : detail::at_impl<meta_object_sequence, position>
{ };

} // namespace mirror
} // namespace boost

#endif //include guard

