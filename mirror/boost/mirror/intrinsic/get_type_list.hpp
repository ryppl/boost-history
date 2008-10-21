/**
 * \file boost/mirror/intrinsic/get_type_list.hpp
 * Gets the list of types from a meta-object-sequence.
 * For meta_attributes it returns the types of reflected attributes,
 * for meta_base_classes it returns the types of base classes, etc.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INTRINSIC_GET_TYPE_LIST_HPP
#define BOOST_MIRROR_INTRINSIC_GET_TYPE_LIST_HPP

#include <boost/mpl/accumulate.hpp>

namespace boost {
namespace mirror {
namespace detail {

	/** Declaration of the default at_impl
	 *  helper template.
	 */
	template <class MetaObjectSequence>
	struct get_type_list_impl { };

	/** Specialization of at_impl<MetaObjectSequence>
	 *  for meta_class_attributes<>
	 */
	template <class Class, class VariantTag>
	struct get_type_list_impl<meta_class_attributes<Class, VariantTag> >
	{
		typedef typename meta_class_attributes<
			Class, 
			VariantTag
		>::type_list type;
	};

	/** Specialization of for_each_impl<MetaObjectSequence>
	 *  for meta_class_all_attributes<>
	 */
	template <class Class, class VariantTag>
	struct get_type_list_impl<meta_class_all_attributes<Class, VariantTag> >
	{
		typedef typename meta_class_all_attributes<
			Class, 
			VariantTag
		>::type_list type;
	};

	/** Specialization of for_each_impl<MetaObjectSequence>
	 *  for meta_base_classes<>
	 */
	template <class MetaInheritance>
	struct get_type_list_impl_get_base_class
	{
		typedef typename MetaInheritance::base_class
			type;
	};
	template <class Class, class VariantTag>
	struct get_type_list_impl<meta_base_classes<Class, VariantTag> >
	{
		typedef typename mpl::accumulate<
			typename meta_base_classes<
				Class, 
				VariantTag
			>::list,
			mpl::vector0<>,
			mpl::push_back<
				mpl::_1,
				get_type_list_impl_get_base_class< mpl::_2>
			>
		>::type type;
		
	};

} // namespace detail

template <class MetaObjectSequence>
struct get_type_list : detail::get_type_list_impl<MetaObjectSequence> { };

} // namespace mirror
} // namespace boost

#endif //include guard

