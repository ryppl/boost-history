// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#include "convenience.hpp"

#include <concepts>

#include <boost/mpl/int.hpp>

//ConceptGCC does not implement recursive concepts or associated type templates yet, therefore
//these concepts are preliminary and incomplete.
namespace boost{namespace fusion{namespace gsoc{
	namespace detail
	{
		template<class Iterator,class IntegralConstant>
			typename result_of::advance_c<Iterator,IntegralConstant::value>::type advance_c_test(const Iterator& iterator,IntegralConstant)
		{
			return advance_c<IntegralConstant::value>(iterator);
		}
	}

	auto concept ForwardIterator<class Iterator,
		class OtherIterator=Iterator,
		int IntegralConstant=0> :
			std::CopyConstructible<Iterator>//, std::HasEqualTo<Iterator,Iterator>, std::HasNotEqualTo<Iterator,Iterator>
	{
		//The iterator must have the same tag (e.g. vector_iterator_tag), otherwise operations on them wont make any sense!
		typename tag=typename Iterator::tag;
		typename otheriterator_tag=typename OtherIterator::tag;
		requires std::SameType<tag,otheriterator_tag>;

		//TODO: The sequence of the underlying iterators should be the same
		//The sequence is not publically exposed though.

		//the computed types of the metafunctions should not exposed publically
		//therefore the detail prefix
		typename detail_next_type=typename result_of::next<Iterator>::type;
		detail_next_type next(const Iterator&);

		typename detail_advance_c_type=typename result_of::advance_c<Iterator,IntegralConstant>::type;
		//detail_advance_c_type advance_c<IntegralConstant>(const Iterator&); is not valid in a concept!
		detail_advance_c_type advance_c_test(const Iterator&,mpl::int_<IntegralConstant>);

		typename detail_distance_type=typename result_of::distance<Iterator,OtherIterator>::type;
		//requires boost::mpl::concepts::Integral<detail_distance_type>;
		detail_distance_type distance(const Iterator&,const OtherIterator&);

		//These operations are only valid for non-end iterators. This concept should
		//be valid for all iterator types, even the end ones, though. What to do?
		//A different concept for non-end iterators?
		//typename detail_deref_type=typename result_of::deref<Iterator>::type;
		//detail_deref_type deref(const Iterator&);

		//typename detail_value_of_type=typename result_of::value_of<Iterator>::type;
	}

	/*auto concept BidirectionalIterator<class Iterator>
	{
		requires ForwardIterator<Type>;
	}

	auto concept RandomAccessIterator<class Iterator>
	{
		requires BidirectionalIterator<Type>;
	}*/

	/*auto concept ForwardSequence<class Type>
	{
	}

	auto concept BidirectionalSequence<class Type>
	{
		requires ForwardSequence<Type>;
	}

	auto concept RandomAccessSequence<class Type>
	{
		requires BidirectionalSequence<Type>;
	}
	*/
}}}
