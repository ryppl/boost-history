/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ITERATOR_RANGE_ITERATOR_RANGE_HPP
#define BOOST_FUSION_VIEW_ITERATOR_RANGE_ITERATOR_RANGE_HPP

#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/category_of.hpp>

#include <boost/mpl/bool.hpp>

#include <boost/fusion/view/iterator_range/detail/iterator_range_fwd.hpp>
#include <boost/fusion/view/iterator_range/detail/begin_impl.hpp>
#include <boost/fusion/view/iterator_range/detail/end_impl.hpp>
#include <boost/fusion/view/iterator_range/detail/at_impl.hpp>
#include <boost/fusion/view/iterator_range/detail/value_at_impl.hpp>

namespace boost { namespace fusion
{
    struct iterator_range_tag;
    struct fusion_sequence_tag;

    template <typename First, typename Last>
    struct iterator_range
      : sequence_base<iterator_range<First, Last> >
    {
        typedef iterator_range_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::true_ is_view;
        typedef typename result_of::distance<First, Last>::type size;

        typedef First begin_type;
        typedef Last end_type;
        typedef typename traits::category_of<First>::type category;

        template<typename OtherIteratorRange>
        iterator_range(BOOST_FUSION_R_ELSE_CLREF(OtherIteratorRange) range)
          : first(range.first)
          , last(range.last)
        {}

        iterator_range(First const& first, Last const& last)
          : first(first)
          , last(last)
        {}

        template<typename OtherIteratorRange>
        iterator_range&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIteratorRange) other_range)
        {
            first=other_range.first;
            last=other_range.last;
            return *this;
        }

        begin_type first;
        end_type last;
    };
}}

#endif
