/**
 * \file boost/mirror/iterator/detail/iterator_comparison.hpp
 *
 * Common boilerplate for iterator comparison.
 * 
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ITERATOR_DETAIL_ITERATOR_COMPARISON_HPP
#define BOOST_MIRROR_ITERATOR_DETAIL_ITERATOR_COMPARISON_HPP

#include <boost/mirror/iterator/detail/common_iterator.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/bool.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <
	class Position1,
	class Position2,
	class Direction,
	template <class, class> class PosCmp
>
struct compare_iterator_positions : PosCmp<
	typename mpl::times<Position1, Direction>::type,
	typename mpl::times<Position2, Direction>::type
> { };


/** Forward declaration of iterator comparator.
 *  It is intentionaly left undefined.
 */
template <
	class I1, 
	class I2,
	template <class, class> class PosCmp
>
struct compare_iterators;

/** Specialization of compare_iterators<I1, I2> for
 *  I1 = meta_object_iterator_base_templ<...>
 *  I2 = meta_object_iterator_base_templ<...>
 */
template <
	class ReflectedType, 
	class VariantTag,
	class MetaObjectSequence,
	class Position1,
	class Position2,
	class BeginPos,
	class EndPos,
	class Direction,
	class UnaryPredicate1,
	class UnaryPredicate2,
	class Selector,
	template <class, class> class PosCmp
>
struct compare_iterators<
	meta_object_iterator_base_templ<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		Position1,
		BeginPos,
		EndPos,
		Direction,
		UnaryPredicate1,
		Selector
	>,
	meta_object_iterator_base_templ<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		Position2,
		BeginPos,
		EndPos,
		Direction,
		UnaryPredicate2,
		Selector
	>,
	PosCmp
> : compare_iterator_positions<
	Position1, 
	Position2, 
	Direction, 
	PosCmp
> { };


/** Specialization of compare_iterators<I1, I2> for
 *  I1 = a concrete Iterator
 *  I2 = meta_object_iterator_base_templ<...>
 */
template <
	template <class, class, class, class, class, class> class Iterator,
	class ReflectedType, 
	class VariantTag,
	class MetaObjectSequence,
	class Position1,
	class Position2,
	class BeginPos,
	class EndPos,
	class Direction,
	class UnaryPredicate1,
	class UnaryPredicate2,
	class Selector,
	template <class, class> class PosCmp
>
struct compare_iterators<
	Iterator<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		Position1,
		UnaryPredicate1,
		Selector
	>,
	meta_object_iterator_base_templ<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		Position2,
		BeginPos,
		EndPos,
		Direction,
		UnaryPredicate2,
		Selector
	>,
	PosCmp
> : compare_iterator_positions<
	Position1, 
	Position2, 
	Direction, 
	PosCmp
> { };

/** Specialization of compare_iterators<I1, I2> for
 *  I1 = meta_object_iterator_base_templ<...>
 *  I2 = a concrete Iterator
 */
template <
	template <class, class, class, class, class, class> class Iterator,
	class ReflectedType, 
	class VariantTag,
	class MetaObjectSequence,
	class Position1,
	class Position2,
	class BeginPos,
	class EndPos,
	class Direction,
	class UnaryPredicate1,
	class UnaryPredicate2,
	class Selector,
	template <class, class> class PosCmp
>
struct compare_iterators<
	meta_object_iterator_base_templ<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		Position1,
		BeginPos,
		EndPos,
		Direction,
		UnaryPredicate1,
		Selector
	>,
	Iterator<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		Position2,
		UnaryPredicate2,
		Selector
	>,
	PosCmp
> : compare_iterator_positions<
	Position1, 
	Position2, 
	Direction, 
	PosCmp
> { };

/** Specialization of compare_iterators<I1, I2> for
 *  I1 = a concrete Iterator
 *  I2 = a concrete Iterator
 */
template <
	template <class, class, class, class, class, class> class Iterator,
	class ReflectedType, 
	class VariantTag,
	class MetaObjectSequence,
	class Position1,
	class Position2,
	class UnaryPredicate1,
	class UnaryPredicate2,
	class Selector,
	template <class, class> class PosCmp
>
struct compare_iterators<
	Iterator<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		Position1,
		UnaryPredicate1,
		Selector
	>,
	Iterator<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		Position2,
		UnaryPredicate2,
		Selector
	>,
	PosCmp
> : compare_iterator_positions<
	Position1, 
	Position2, 
	typename Iterator<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		Position1,
		UnaryPredicate1,
		Selector
	>::direction,
	PosCmp
> { };


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

