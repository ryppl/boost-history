// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#include <concepts>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/bool.hpp>

namespace boost{namespace fusion{namespace gsoc{
    //Fusion-iterators and sequences are defined through a set of (meta-)functions rather than their own public interface.
    //Unfortunately some of these (meta-)functions are not valid for specific valid types (e.g. next is invalid on an end
    //iterator). We also need arguments for most (meta-)functions, which cannot be provided easily in the context of a concept.
    //Therefore we ignore these (meta-)functions here.

    namespace detail
    {
        typedef mpl::vector<forward_iterator_category,bidirectional_iterator_category,random_access_iterator_category> forward_iterator_categories;
        typedef mpl::vector<bidirectional_iterator_category,random_access_iterator_category> bidirectional_iterator_categories;
        typedef mpl::vector<random_access_iterator_category> random_access_iterator_categories;
    }

    auto concept ForwardIterator<class Iterator> :
            std::CopyConstructible<Iterator>//, std::HasEqualTo<Iterator,Iterator>, std::HasNotEqualTo<Iterator,Iterator>
    {
        typename tag=typename Iterator::tag;
        typename category=typename Iterator::category;

        //requires std::SameType<mpl::contains<detail::forward_iterator_categories,tag>::type,mpl::true_>; segfaults in ConceptGCC!
        typename detail_is_valid=typename mpl::contains<detail::forward_iterator_categories,category>::type;
        requires std::SameType<detail_is_valid,mpl::true_>;
    }

    auto concept BidirectionalIterator<class Iterator> : ForwardIterator<Iterator>
    {
        typename detail_is_valid=typename mpl::contains<detail::bidirectional_iterator_categories,category>::type;
        requires std::SameType<detail_is_valid,mpl::true_>;
    }

    auto concept RandomAccessIterator<class Iterator> : BidirectionalIterator<Iterator>
    {
        typename detail_is_valid=typename mpl::contains<detail::random_access_iterator_categories,category>::type;
        requires std::SameType<detail_is_valid,mpl::true_>;
    }

    namespace detail
    {
        typedef mpl::vector<forward_sequence_category,bidirectional_sequence_category,random_access_sequence_category> forward_sequence_categories;
        typedef mpl::vector<bidirectional_sequence_category,random_access_sequence_category> bidirectional_sequence_categories;
        typedef mpl::vector<random_access_sequence_category> random_access_sequence_categories;
    }

    auto concept ForwardSequence<class Sequence> :
        std::CopyConstructible<Sequence>//, std::HasEqualTo<Sequence,Sequence>, std::HasNotEqualTo<Sequence,Sequence>
    {
        typename tag=typename Sequence::tag;
        typename category=typename Sequence::category;

        typename detail_is_valid=typename mpl::contains<detail::forward_sequence_categories,category>::type;
        requires std::SameType<detail_is_valid,mpl::true_>;
    }

    auto concept BidirectionalSequence<class Sequence> : ForwardSequence<Sequence>
    {
        typename detail_is_valid=typename mpl::contains<detail::bidirectional_sequence_categories,category>::type;
        requires std::SameType<detail_is_valid,mpl::true_>;
    }

    auto concept RandomAccessSequence<class Sequence> : BidirectionalSequence<Sequence>
    {
        typename detail_is_valid=typename mpl::contains<detail::random_access_sequence_categories,category>::type;
        requires std::SameType<detail_is_valid,mpl::true_>;
    }
}}}
