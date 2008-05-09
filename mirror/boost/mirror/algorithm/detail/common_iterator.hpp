/**
 * \file boost/mirror/algorithm/detail/common_iterator.hpp
 *
 *  Common iterator-related declarations
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_COMMON_ITERATOR_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_COMMON_ITERATOR_HPP

#include <boost/mirror/algorithm/size.hpp>
#include <boost/mirror/algorithm/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/equal_to.hpp>



namespace boost {
namespace mirror {

/** Dereferences an iterator
 */
template <class Iterator>
struct deref
{
	typedef typename Iterator::pointed_to
		type;
};

namespace detail {

	template <class MetaObjectSequence>
	struct iterator_pointee_selector;

	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class Position,
		class UnaryPredicate,
		class Selector

	>
	struct meta_object_iterator_base
	{
		typedef Position position;

		// the type meta_class_attribute
		// that this iterator points to
		typedef typename Selector::apply<
			ReflectedType, 
			VariantTag,
			MetaObjectSequence,
			Position,
			UnaryPredicate
		>::type pointed_to;


		// the current iterator getter
		struct get_this_iterator
		{
			template <typename DummyPosition>
			struct apply
			{
				typedef meta_object_iterator_base<
					ReflectedType, 
					VariantTag,
					MetaObjectSequence,
					DummyPosition,
					UnaryPredicate,
					Selector
				> type;
			};
		};

		// the prior iterator getter
		struct get_prior_iterator
		{
			template <typename DummyPosition>
			struct apply
			{
				typedef meta_object_iterator_base<
					ReflectedType, 
					VariantTag,
					MetaObjectSequence,
					mpl::int_<DummyPosition::value - 1>,
					UnaryPredicate,
					Selector
				> type;
			};
		};


		// the initial or next iterator getter
		template <int I>
		struct get_initial_or_next_iterator
		{
			typedef mpl::int_<size<MetaObjectSequence>::value > end_pos;

			template <typename DummyPosition>
			struct apply
			{
				typedef meta_object_iterator_base<
					ReflectedType, 
					VariantTag,
					MetaObjectSequence,
					mpl::int_<DummyPosition::value + I>,
					UnaryPredicate,
					Selector
				> next_iterator;

				typedef typename mpl::apply<
					UnaryPredicate, 
					typename deref<next_iterator>::type
				>::type next_is_valid;

				// TODO: this needs to be optimized.
				typedef typename mpl::if_<
					next_is_valid,
					next_iterator,
					typename mpl::apply<
						typename next_iterator::get_next_iterator,
						typename next_iterator::position
					>::type
				> ::type type;
			}; 

			template <>
			struct apply<end_pos>
			{
				typedef meta_object_iterator_base<
					ReflectedType, 
					VariantTag,
					MetaObjectSequence,
					end_pos,
					UnaryPredicate,
					Selector
				> type;
			}; 
		}; 

		struct get_initial_iterator : get_initial_or_next_iterator<0>{ };
		struct get_next_iterator : get_initial_or_next_iterator<1>{ };

	}; // meta_object_iterator_base



} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

