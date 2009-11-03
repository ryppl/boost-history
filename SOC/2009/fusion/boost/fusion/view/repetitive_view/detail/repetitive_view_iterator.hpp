/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_REPETITIVE_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_REPETITIVE_VIEW_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/mpl/int.hpp>

namespace boost { namespace fusion
{
    struct repetitive_view_iterator_tag;

    template<typename SeqRef, typename It, int Index>
    struct repetitive_view_iterator
      : iterator_base<repetitive_view_iterator<SeqRef, It, Index> >
    {
        typedef SeqRef seq_type;
        typedef It it_type;
        typedef mpl::int_<Index> index;

        typedef repetitive_view_iterator_tag fusion_tag;
        typedef typename traits::category_of<It>::type category;

        template<typename OtherIt>
        repetitive_view_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : seq(BOOST_FUSION_FORWARD(OtherIt,it).seq)
          , it(BOOST_FUSION_FORWARD(OtherIt,it).it)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,repetitive_view_iterator_tag);
        }

        repetitive_view_iterator(SeqRef seq, It const& it)
          : seq(&seq)
          , it(it)
        {}

        template<typename OtherIt>
        repetitive_view_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it_)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,repetitive_view_iterator_tag);

            seq=BOOST_FUSION_FORWARD(OtherIt,it_).seq;
            it=BOOST_FUSION_FORWARD(OtherIt,it_).it;
            return *this;
        }

        typename detail::remove_reference<seq_type>::type* seq;
        It it;
    };
}}

#endif
