/**
 * \file boost/mirror/algorithm/is_equal_iterator.hpp
 * 
 * Iterator equality comparison
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_IS_EQUAL_ITERATOR_HPP
#define BOOST_MIRROR_ALGORITHM_IS_EQUAL_ITERATOR_HPP

#include <boost/mirror/algorithm/detail/iterator.hpp>
#include <boost/mpl/bool.hpp>

namespace boost {
namespace mirror {

template <class U, class V>
struct is_equal_iterator : mpl::false_ { };


template <
	class ReflectedType, 
	class VariantTag,
	class MetaAttributes,
	class AttribPos,
	class UnaryPredicate1,
	class UnaryPredicate2,
	class Selector
>
struct is_equal_iterator<
	detail::meta_object_iterator<
		ReflectedType, 
		VariantTag,
		MetaAttributes,
		AttribPos,
		UnaryPredicate1
	>,
	detail::meta_object_iterator_base<
		ReflectedType, 
		VariantTag,
		MetaAttributes,
		AttribPos,
		UnaryPredicate2,
		Selector
	>
> : mpl::true_ { };

template <
	class ReflectedType, 
	class VariantTag,
	class MetaAttributes,
	class AttribPos,
	class UnaryPredicate1,
	class UnaryPredicate2
>
struct is_equal_iterator<
	detail::meta_object_iterator<
		ReflectedType, 
		VariantTag,
		MetaAttributes,
		AttribPos,
		UnaryPredicate1
	>,
	detail::meta_object_iterator<
		ReflectedType, 
		VariantTag,
		MetaAttributes,
		AttribPos,
		UnaryPredicate2
	>
> : mpl::true_ { };


} // namespace mirror
} // namespace boost

#endif //include guard

