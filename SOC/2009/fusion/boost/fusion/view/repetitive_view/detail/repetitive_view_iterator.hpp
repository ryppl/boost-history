// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_REPETITIVE_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_REPETITIVE_VIEW_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/category_of.hpp>

#include <boost/mpl/int.hpp>

namespace boost { namespace fusion
{
    struct repetitive_view_iterator_tag;

    template<
        typename SeqRef
      , typename It
      , int Index
    >
    struct repetitive_view_iterator
      : iterator_base<repetitive_view_iterator<SeqRef, It, Index> >
    {
        typedef SeqRef seq_type;
        typedef It it_type;
        typedef mpl::int_<Index> index;

        typedef repetitive_view_iterator_tag fusion_tag;
        typedef typename traits::category_of<It>::type category;

        repetitive_view_iterator(It const& it)
          : it(it)
        {}

        It it;
    };
}}

#endif
