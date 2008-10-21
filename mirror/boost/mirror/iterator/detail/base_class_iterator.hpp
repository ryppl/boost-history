/**
 * \file boost/mirror/iterator/detail/base_class_iterator.hpp
 * Iterators for iterating through meta-object sequences
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ITERATOR_DETAIL_BASE_CLASS_ITERATOR_HPP
#define BOOST_MIRROR_ITERATOR_DETAIL_BASE_CLASS_ITERATOR_HPP

#include <boost/mirror/iterator/detail/common_iterator.hpp>
#include <boost/mirror/meta_inheritance.hpp>


namespace boost {
namespace mirror {
namespace detail {

	/** Iterator for iterating through meta_base_classes
	 */
	template <
		class _ReflectedType, 
		class _VariantTag
	>
	struct iterator_pointee_selector<
		meta_base_classes<_ReflectedType, _VariantTag>
	>
	{
		template <
			class ReflectedType, 
			class VariantTag,
			class MetaBaseClasses,
			class BaseClassPos,
			class UnaryPredicate
		>
		struct apply
		{
			typedef typename at<
				MetaBaseClasses,
				BaseClassPos
			>::type type;
		};
	};


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

