/**
 * \file boost/mirror/iterator/detail/common_iterator.hpp
 *
 *  Common iterator-related declarations
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ITERATOR_DETAIL_COMMON_ITERATOR_HPP
#define BOOST_MIRROR_ITERATOR_DETAIL_COMMON_ITERATOR_HPP

#include <boost/mirror/intrinsic/size.hpp>
#include <boost/mirror/intrinsic/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/comparison.hpp>



namespace boost {
namespace mirror {

/** Dereferences an iterator
 */
template <class Iterator>
struct deref
{
	typedef typename Iterator::template get_pointed_to<void>::type
		type;
};

namespace detail {

	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class Position,
		class BeginPos,
		class EndPos,
		class Direction,
		class UnaryPredicate,
		class Selector
	>
	struct meta_object_iterator_base_templ;

	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class DummyPosition,
		class BeginPos,
		class EndPos,
		class Direction,
		class UnaryPredicate,
		class Selector,
		int I
	>
	struct meta_object_iterator_base_templ_get_ioni_apply
	{
		typedef meta_object_iterator_base_templ<
			ReflectedType, 
			VariantTag,
			MetaObjectSequence,
			typename mpl::int_<mpl::plus<
				DummyPosition,
				mpl::times<
					Direction,
					mpl::int_<I>
				>
			>::value>,
			BeginPos,
			EndPos,
			Direction,
			UnaryPredicate,
			Selector
		> next_iterator;

		typedef typename mpl::apply<
			UnaryPredicate, 
			typename deref<next_iterator>::type
		>::type next_is_valid;

		// TODO: this should be optimized.
		typedef typename mpl::if_<
			next_is_valid,
			next_iterator,
			typename mpl::apply<
				typename next_iterator::get_next_iterator,
				typename next_iterator::position
			>::type
		> ::type type;
	};

	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class BeginPos,
		class EndPos,
		class Direction,
		class UnaryPredicate,
		class Selector,
		int I
	>
	struct meta_object_iterator_base_templ_get_ioni_apply<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		EndPos,
		BeginPos,
		EndPos,
		Direction,
		UnaryPredicate,
		Selector,
		I
	>
	{
		typedef meta_object_iterator_base_templ<
			ReflectedType, 
			VariantTag,
			MetaObjectSequence,
			EndPos,
			BeginPos,
			EndPos,
			Direction,
			UnaryPredicate,
			Selector
		> type;
	};

	/** Forward declaration of iterator_pointee_selector
	 */
	template <class MetaObjectSequence>
	struct iterator_pointee_selector;

	/** Basic template for the meta-object 
	 *  iterators
	 */
	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class Position,
		class BeginPos,
		class EndPos,
		class Direction,
		class UnaryPredicate,
		class Selector
	>
	struct meta_object_iterator_base_templ
	{

		typedef Position position;
		typedef BeginPos begin_position;
		typedef EndPos end_position;

		typedef Direction direction;

		template <typename Dummy>
		struct get_pointed_to
		{
			// the type meta_class_attribute
			// that this iterator points to
			typedef typename Selector::template apply<
				ReflectedType, 
				VariantTag,
				MetaObjectSequence,
				Position,
				UnaryPredicate
			>::type type;
		};

		// the current iterator getter
		struct get_this_iterator
		{
			template <typename DummyPosition>
			struct apply
			{
				typedef meta_object_iterator_base_templ<
					ReflectedType, 
					VariantTag,
					MetaObjectSequence,
					DummyPosition,
					BeginPos,
					EndPos,
					Direction,
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
				typedef meta_object_iterator_base_templ<
					ReflectedType, 
					VariantTag,
					MetaObjectSequence,
					typename mpl::int_<mpl::minus<
						DummyPosition,
						mpl::times<
							Direction,
							mpl::int_<1>
						>
					>::value>,
					BeginPos,
					EndPos,
					Direction,
					UnaryPredicate,
					Selector
				> type;
			};
		};


		// the initial or next iterator getter
		template <int I>
		struct get_initial_or_next_iterator
		{

			template <typename DummyPosition>
			struct apply : meta_object_iterator_base_templ_get_ioni_apply<
				ReflectedType, 
				VariantTag,
				MetaObjectSequence,
				DummyPosition,
				BeginPos,
				EndPos,
				Direction,
				UnaryPredicate,
				Selector,
				I
			> { }; 

		}; 

		struct get_initial_iterator : get_initial_or_next_iterator<0>{ };
		struct get_next_iterator : get_initial_or_next_iterator<1>{ };

	}; // meta_object_iterator_base

	/** Forward iterator base template
	 */
	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class Position,
		class UnaryPredicate,
		class Selector
	>
	struct meta_object_iterator_base
	: meta_object_iterator_base_templ<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		Position,
		mpl::int_<0>,
		mpl::int_<size<MetaObjectSequence>::value >,
		mpl::int_<1>,
		UnaryPredicate,
		Selector
	>{ };

	/** Reverse iterator base class
	 */
	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class Position,
		class UnaryPredicate,
		class Selector
	>
	struct meta_object_reverse_iterator_base
	: meta_object_iterator_base_templ<
		ReflectedType, 
		VariantTag,
		MetaObjectSequence,
		Position,
		mpl::int_<size<MetaObjectSequence>::value -1 >,
		mpl::int_<-1>,
		mpl::int_<-1>,
		UnaryPredicate,
		Selector
	>{ };


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

