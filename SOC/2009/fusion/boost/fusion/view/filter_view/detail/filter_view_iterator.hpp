/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_FILTER_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_FILTER_VIEW_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/algorithm/query/detail/find_if.hpp>

namespace boost { namespace fusion
{
    struct filter_view_iterator_tag;
    struct forward_traversal_tag;

    template <typename First, typename Last, typename Pred>
    struct filter_iterator
      : iterator_base<filter_iterator<First, Last, Pred> >
    {
        typedef filter_view_iterator_tag fusion_tag;
        typedef forward_traversal_tag category;
        typedef detail::static_find_if<First, Last, Pred> filter;
        typedef typename filter::type first_type;
        typedef Last last_type;
        typedef Pred pred_type;

        template<typename OtherIt>
        filter_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : first(BOOST_FUSION_FORWARD(OtherIt,it).first)
        {}

        filter_iterator(First const& first,int)
          : first(filter::call(first))
        {}

        template<typename OtherIt>
        filter_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
        {
            first=BOOST_FUSION_FORWARD(OtherIt,it).first;
            return *this;
        }

        first_type first;
    };
}}

#endif
