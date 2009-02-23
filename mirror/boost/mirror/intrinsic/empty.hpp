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

	/** Declaration of the default empty_impl
	 *  helper template.
	 */
	template <class MetaObjectSequence>
	struct empty_impl { };

	/** Specialization of empty_impl<MetaObjectSequence>
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

	/** Specialization of empty_impl<MetaObjectSequence>
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

	/** Specialization of empty_impl<MetaObjectSequence>
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

	/** Specialization of empty_impl<MetaObjectSequence>
	 *  for global_list<>
	 */
	template <class Selector, class Counter>
	struct empty_impl<counter::global_list<Selector, Counter> >
	: mpl::empty<
			typename counter::global_list<
				Selector, 
				Counter	
			>::type_list
	>{ };

} // namespace detail

template <class MetaObjectSequence>
struct empty : detail::empty_impl<MetaObjectSequence> { };

} // namespace mirror
} // namespace boost

#endif //include guard

