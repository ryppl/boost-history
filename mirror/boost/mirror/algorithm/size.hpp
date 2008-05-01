/**
 * \file boost/mirror/algorithm/size.hpp
 * Gets count of meta-attributes/meta-inheritences/etc.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_SIZE_HPP
#define BOOST_MIRROR_ALGORITHM_SIZE_HPP

namespace boost {
namespace mirror {
namespace detail {

	/** Declaration of the default at_impl
	 *  helper template.
	 */
	template <class meta_object_sequence>
	struct size_impl { };

	/** Specialization of at_impl<meta_object_sequence>
	 *  for meta_class_attributes<>
	 */
	template <class reflected_class, class variant_tag>
	struct size_impl<meta_class_attributes<reflected_class, variant_tag> >
	: mpl::size<
			typename meta_class_attributes<
				reflected_class, 
				variant_tag
			>::type_list
	>{ };

	/** Specialization of for_each_impl<meta_object_sequence>
	 *  for meta_class_all_attributes<>
	 */
	template <class reflected_class, class variant_tag>
	struct size_impl<meta_class_all_attributes<reflected_class, variant_tag> >
	: mpl::size<
			typename meta_class_all_attributes<
				reflected_class, 
				variant_tag
			>::type_list
	>{ };

	/** Specialization of for_each_impl<meta_object_sequence>
	 *  for meta_base_classes<>
	 */
	template <class reflected_class, class variant_tag>
	struct size_impl<meta_base_classes<reflected_class, variant_tag> >
	: mpl::size<
			typename meta_base_classes<
				reflected_class, 
				variant_tag
			>::list
	>{ };

} // namespace detail

template <class meta_object_sequence>
struct size : detail::size_impl<meta_object_sequence> { };

} // namespace mirror
} // namespace boost

#endif //include guard

