/**
 * \file boost/mirror/intrinsic/empty.hpp
 * Gets count of meta-attributes/meta-inheritences/etc.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INTRINSIC_EMPTY_HPP
#define BOOST_MIRROR_INTRINSIC_EMPTY_HPP

#include <boost/mpl/empty.hpp>

#include <boost/mirror/meta_attributes.hpp>
#include <boost/mirror/meta_inheritance.hpp>

namespace boost {
namespace mirror {
namespace detail {

	/** Declaration of the default at_impl
	 *  helper template.
	 */
	template <class MetaObjectSequence>
	struct empty_impl { };

	/** Specialization of at_impl<MetaObjectSequence>
	 *  for meta_class_attributes<>
	 */
	template <class Class, class VariantTag>
	struct empty_impl<meta_class_attributes<Class, VariantTag> >
	: mpl::empty<
			typename meta_class_attributes<
				Class, 
				VariantTag
			>::type_list
	>{ };

	/** Specialization of for_each_impl<MetaObjectSequence>
	 *  for meta_class_all_attributes<>
	 */
	template <class Class, class VariantTag>
	struct empty_impl<meta_class_all_attributes<Class, VariantTag> >
	: mpl::empty<
			typename meta_class_all_attributes<
				Class, 
				VariantTag
			>::type_list
	>{ };

	/** Specialization of for_each_impl<MetaObjectSequence>
	 *  for meta_base_classes<>
	 */
	template <class Class, class VariantTag>
	struct empty_impl<meta_base_classes<Class, VariantTag> >
	: mpl::empty<
			typename meta_base_classes<
				Class, 
				VariantTag
			>::list
	>{ };

} // namespace detail

template <class MetaObjectSequence>
struct empty : detail::empty_impl<MetaObjectSequence> { };

} // namespace mirror
} // namespace boost

#endif //include guard

