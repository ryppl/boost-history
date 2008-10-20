/**
 * \file boost/mirror/algorithm/detail/positional.hpp
 *
 * Common boilerplate for iterator getters.
 * 
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_POSITIONAL_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_POSITIONAL_HPP

#include <boost/mirror/meta_attributes.hpp>
#include <boost/mirror/meta_inheritance.hpp>

namespace boost {
namespace mirror {
namespace detail {

	/** Declaration of the default position_getter_impl_1
	 *  helper template.
	 */
	template <
		class MetaObjectSequence, 
		class Arg1, 
		template <class, class, class, class> class Implementation
	>
	struct position_getter_impl_1 { };

	/** Specialization of position_getter_impl_1<MetaObjectSequence>
	 *  for meta_class_attributes<>
	 */
	template <
		class Class, 
		class VariantTag, 
		class Arg1,
		template <class, class, class, class> class Implementation
	>
	struct position_getter_impl_1<
		meta_class_attributes<Class, VariantTag>,
		Arg1,
		Implementation 
	>
	: Implementation<
		Class, VariantTag,
		meta_class_attributes<Class, VariantTag>,
		Arg1
	>{ };

	/** Specialization of for_each_impl_1<MetaObjectSequence>
	 *  for meta_class_all_attributes<>
	 */
	template <
		class Class, 
		class VariantTag, 
		class Arg1,
		template <class, class, class, class> class Implementation
	>
	struct position_getter_impl_1<
		meta_class_all_attributes<Class, VariantTag>,
		Arg1,
		Implementation 
	>
	: Implementation<
		Class, VariantTag,
		meta_class_all_attributes<Class, VariantTag>,
		Arg1
	>{ };

	/** Specialization of position_getter_impl_1<MetaObjectSequence>
	 *  for meta_base_classes<>
	 */
	template <
		class Class, 
		class VariantTag,
		class Arg1,
		template <class, class, class, class> class Implementation
	>
	struct position_getter_impl_1<
		meta_base_classes<Class, VariantTag>, 
		Arg1,
		Implementation 
	>
	: Implementation<
		Class, VariantTag,
		meta_base_classes<Class, VariantTag>,
		Arg1
	>{ };

	/** Declaration of the default position_getter_impl
	 *  helper template.
	 */
	template <
		class MetaObjectSequence, 
		template <class, class, class, class> class Implementation
	>
	struct position_getter_impl
	: position_getter_impl_1<MetaObjectSequence, void, Implementation>
	{ };

	/** Declaration of the default explicit_position_getter_impl
	 *  helper template.
	 */
	template <
		class MetaObjectSequence, 
		class Position,
		template <class, class, class, class> class Implementation
	>
	struct explicit_position_getter_impl
	: position_getter_impl_1<MetaObjectSequence, Position, Implementation>
	{ };



} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

