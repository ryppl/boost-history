/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ITERATOR_RANGE_ITERATOR_RANGE_HPP
#define BOOST_FUSION_VIEW_ITERATOR_RANGE_ITERATOR_RANGE_HPP

#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/fusion/view/iterator_range/detail/iterator_range_fwd.hpp>
#include <boost/fusion/view/iterator_range/detail/begin_impl.hpp>
#include <boost/fusion/view/iterator_range/detail/end_impl.hpp>
#include <boost/fusion/view/iterator_range/detail/at_impl.hpp>
#include <boost/fusion/view/iterator_range/detail/value_at_impl.hpp>

namespace boost { namespace fusion
{
    struct iterator_range_tag;
    struct fusion_sequence_tag;

    template <typename Begin, typename End>
    struct iterator_range
      : sequence_base<iterator_range<Begin, End> >
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_iterator<Begin>));
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Begin>));
        BOOST_FUSION_MPL_ASSERT((traits::is_iterator<End>));
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<End>));

        typedef Begin begin_type;
        typedef End end_type;

        typedef typename traits::category_of<Begin>::type category;
        typedef typename result_of::distance<begin_type, end_type>::type size;
        typedef iterator_range_tag fusion_tag;
        typedef fusion_sequence_tag tag;
        typedef mpl::true_ is_view;

        template<typename OtherRange>
        iterator_range(BOOST_FUSION_R_ELSE_CLREF(OtherRange) range)
          : first(range.first)
          , last(range.last)
        {
            BOOST_FUSION_TAG_CHECK(OtherRange,iterator_range_tag);
        }

        iterator_range(Begin const& first, End const& last)
          : first(first)
          , last(last)
        {}

        template<typename OtherRange>
        iterator_range&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherRange) other_range)
        {
            BOOST_FUSION_TAG_CHECK(OtherRange,iterator_range_tag);

            first=other_range.first;
            last=other_range.last;
            return *this;
        }

        begin_type first;
        end_type last;
    };
}}

#endif
